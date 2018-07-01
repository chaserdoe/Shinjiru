#include "torrents.h"
#include "ui_torrents.h"

#include <QDesktopServices>
#include <QEventLoop>
#include <QFile>
#include <QMenu>

#include "../../../src/clients/nekomimi.h"
#include "../../../src/paths.h"
#include "../../../src/utilities/file_downloader.h"
#include "../../settings_dialog.h"

namespace Views {

Torrents::Torrents(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Torrents),
      model(new RSSTableModel(this)),
      proxy_model(new RSSTableProxyModel(this)),
      timer(new QTimer(this)) {
  ui->setupUi(this);
  ui->torrentTable->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);

  model->setList(items);
  timer->setInterval(1000);

  proxy_model->setSourceModel(model);

  ui->torrentTable->setModel(proxy_model);
  ui->torrentTable->horizontalHeader()->setStretchLastSection(true);
  ui->torrentTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->torrentTable->verticalHeader()->setDefaultSectionSize(
      ui->torrentTable->fontMetrics().height() + 8);
  ui->torrentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->torrentTable->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(ui->torrentFilter, &QLineEdit::textChanged, this,
          [this](const QString &text) { proxy_model->setFilter(text); });

  connect(ui->refreshButton, &QPushButton::clicked, this, [this]() { this->fetchTorrents(); });

  connect(ui->torrentTable, &QTableView::doubleClicked, this,
          [this](const QModelIndex index) { this->download(model->item(index)); });

  connect(ui->torrentTable, &QTableView::customContextMenuRequested, this, &Torrents::contextMenu);

  connect(ui->clearSortButton, &QPushButton::clicked, this, [this]() {
    ui->torrentTable->horizontalHeader()->blockSignals(true);
    ui->torrentTable->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    proxy_model->sort(-1);
    ui->torrentTable->horizontalHeader()->blockSignals(false);
  });

  connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

  timer->start();
}

Torrents::~Torrents() {
  delete ui;
}

void Torrents::timerTick() {
  --refresh;

  if (refresh <= 0) {
    fetchTorrents();
    refresh = 900;
  }

  ui->refreshButton->setText(tr("Refresh (%1)").arg(refresh));
}

void Torrents::contextMenu() {
  QPoint point = QCursor::pos();

  auto selectionModel = ui->torrentTable->selectionModel();
  auto indexes = selectionModel->selectedIndexes();
  auto row = indexes[0].row();
  auto index = proxy_model->index(row, RSSRoles::Title);
  auto sourceIndex = proxy_model->mapToSource(index);

  auto rssItem = model->item(sourceIndex);

  auto contextMenu = new QMenu;

  auto downloadItem = new QAction(tr("Download"), contextMenu);
  auto createRuleItem = new QAction(tr("Create Rule"), contextMenu);

  contextMenu->addAction(downloadItem);
  contextMenu->addAction(createRuleItem);

  connect(downloadItem, &QAction::triggered, this, [this, rssItem]() { download(rssItem); });
  connect(createRuleItem, &QAction::triggered, this, [this, rssItem]() {
    if (rssItem->mediaId > 0) {
      SettingsDialog *dialog = new SettingsDialog;

      connect(dialog, &SettingsDialog::finished, this, [dialog]() { dialog->deleteLater(); });

      dialog->show();
      dialog->addTorrentItem(rssItem);
    } else {
      qWarning() << "Unknown media for " << rssItem->title;
    }
  });

  contextMenu->exec(point);
  contextMenu->deleteLater();
}

void Torrents::fetchTorrents() {
  if (!refreshLock.tryLock()) {
    return;
  }

  ui->refreshButton->setEnabled(false);

  for (auto &&item : items) {
    delete item;
  }

  items = Nekomimi::instance().fetch();

  model->setList(items);
  ui->torrentTable->resizeColumnsToContents();

  ui->refreshButton->setEnabled(true);
  refreshLock.unlock();

  checkForMatches();
}

void Torrents::checkForMatches() {
  QFile rules(Paths::configFile("torrent_rules.json"));

  if (!rules.exists()) {
    rules.open(QFile::WriteOnly);
    rules.write("{}");
    rules.close();
  }

  rules.open(QFile::ReadOnly);
  QJsonDocument doc = QJsonDocument::fromJson(rules.readAll());
  QJsonObject obj = doc.object();

  for (auto it = obj.begin(); it != obj.end(); ++it) {
    auto key = it.key();
    auto data = it.value().toArray();

    for (auto &&rule : data) {
      auto ruleObject = rule.toObject();

      checkRule(ruleObject);
    }
  }
}

void Torrents::checkRule(QJsonObject rule) {
  auto mediaId = rule["id"].toInt();
  auto subGroup = rule["subGroup"].toString();
  auto quality = rule["quality"].toString();

  for (auto &&item : this->items) {
    if (mediaId == item->mediaId && subGroup == item->subGroup && quality == item->quality) {
      downloadOnce(item);
    }
  }
}

void Torrents::downloadOnce(RSSItem *item) {
  QString file = item->fileName + ".dl";
  QDir history_dir(Paths::configDir("torrent_history"));
  QFile f(history_dir.absoluteFilePath(file));

  if (!f.exists()) {
    qDebug() << "Downloading" << item->fileName << "from torrent rule";

    if (this->tray) {
      auto msg = tr("Downloading %1 from torrent rule").arg(qPrintable(item->fileName));
      tray->showMessage("Shinjiru", msg);
    }

    download(item);

    f.open(QFile::WriteOnly);
    f.write("0");
    f.close();
  }
}

void Torrents::download(RSSItem *item) {
  qDebug() << "Downloading " + item->fileName;
  FileDownloader f(item->link);

  QEventLoop evt;
  connect(&f, &FileDownloader::downloaded, &evt, &QEventLoop::quit);
  evt.exec();

  QFile temp(QDir::tempPath() + "/" + item->fileName + ".torrent");
  temp.open(QFile::WriteOnly);
  temp.write(f.downloadedData());
  temp.close();

  QDesktopServices::openUrl(QUrl::fromLocalFile(temp.fileName()));
}

void Torrents::setTrayIcon(QSystemTrayIcon *icon) {
  this->tray = icon;
}
}  // namespace Views

#ifndef UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__
#define UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__

#include "../../../src/models/rss_item.h"
#include "../components/committable_widget.h"
#include "../components/torrent_rule.h"
#include "../components/torrent_rule_model.h"

namespace Ui {
class TorrentSettings;
}

namespace Views {

class TorrentSettings : public CommittableWidget {
  Q_OBJECT

 public:
  explicit TorrentSettings(QWidget *parent = 0);
  ~TorrentSettings();

  void resetToDefault() override;
  void commit() override;
  void addItem(RSSItem *item);

 private:
  Ui::TorrentSettings *ui;
  TorrentRuleModel *model;
  TorrentRule *currentRule;
};
}  // namespace Views

#endif  // UI_SETTINGS_DIALOG_VIEWS_TORRENTS_H__

#ifndef UI_MAIN_WINDOW_COMPONENTS_RSS_TABLE_PROXY_MODEL_H__
#define UI_MAIN_WINDOW_COMPONENTS_RSS_TABLE_PROXY_MODEL_H__

#include <QSortFilterProxyModel>

class RSSTableProxyModel : public QSortFilterProxyModel {
  Q_OBJECT

 public:
  explicit RSSTableProxyModel(QObject *parent = 0);
  void setFilter(const QString &filter);

 protected:
  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
  bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

 private:
  QString m_filter;
};

#endif  // UI_MAIN_WINDOW_COMPONENTS_RSS_TABLE_PROXY_MODEL_H__

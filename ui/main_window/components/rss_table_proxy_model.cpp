#include "./rss_table_proxy_model.h"

#include <functional>

#include "./rss_table_model.h"

RSSTableProxyModel::RSSTableProxyModel(QObject *p) : QSortFilterProxyModel(p) {}

void RSSTableProxyModel::setFilter(const QString &filter) {
  m_filter = filter;
  this->invalidateFilter();
}

bool RSSTableProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const {
  if (m_filter.isEmpty()) {
    return true;
  }

  auto source = static_cast<RSSTableModel *>(sourceModel());
  QModelIndex index = source->index(row, 0, parent);

  auto item = source->item(index);

  return item->title.contains(m_filter, Qt::CaseInsensitive);
}

bool RSSTableProxyModel::lessThan(const QModelIndex &l, const QModelIndex &r) const {
  auto source = static_cast<RSSTableModel *>(sourceModel());
  auto column = l.column();

  auto leftItem = source->item(l);
  auto rightItem = source->item(r);

  if (column == RSSRoles::Title) {
    return leftItem->title < rightItem->title;
  } else if (column == RSSRoles::Episode) {
    return leftItem->episode < rightItem->episode;
  } else if (column == RSSRoles::Quality) {
    return leftItem->quality < rightItem->quality;
  } else if (column == RSSRoles::SubGroup) {
    return leftItem->subGroup < rightItem->subGroup;
  } else if (column == RSSRoles::Size) {
    return leftItem->size < rightItem->size;
  } else if (column == RSSRoles::Link) {
    return leftItem->link < rightItem->link;
  }

  return leftItem->title < rightItem->title;
}

#ifndef RECORDINDEX_HXX
#define RECORDINDEX_HXX

#include <functional>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <QModelIndex>
// #include "models/record_table/RecordModel.h"

class TreeItem;
class RecordModel;


struct pos_proxy;
struct pos_source;
struct index_proxy;
struct index_source;
struct id_value;



BOOST_STRONG_TYPEDEF(QString, id_value)
BOOST_STRONG_TYPEDEF(int, pos_source)
BOOST_STRONG_TYPEDEF(int, pos_proxy)
BOOST_STRONG_TYPEDEF(QModelIndex, index_proxy)
BOOST_STRONG_TYPEDEF(QModelIndex, index_source)

struct RecordIndex : public boost::intrusive_ref_counter<RecordIndex, boost::thread_safe_counter> {
    public:

	std::function<RecordModel *()>	current_model() const;
	index_source			sibling_index() const;
	void				sibling_index(const QModelIndex &si);
//	QModelIndex current_index() const;
//    boost::intrusive_ptr<TreeItem> target_sibling() const;
	boost::intrusive_ptr<TreeItem> host() const;

	static boost::intrusive_ptr<RecordIndex>		instance(const std::function<RecordModel *()> &current_model_, boost::intrusive_ptr<TreeItem>  host_, boost::intrusive_ptr<TreeItem> sibling_item_ = nullptr);
//	static boost::intrusive_ptr<RecordIndex>		instance(const std::function<RecordModel *()> &current_model_, boost::intrusive_ptr<TreeItem>  host_, const index_source &sibling_index);
	static boost::intrusive_ptr<TreeItem>			synchronize(boost::intrusive_ptr<TreeItem>  host_) noexcept;
	boost::intrusive_ptr<TreeItem>				bind(bool make_current = true) noexcept;
    private:
//	RecordIndex(const std::function<RecordModel *()> &current_model, boost::intrusive_ptr<TreeItem>  target_item, boost::intrusive_ptr<TreeItem> sibling_item);
	RecordIndex(const std::function<RecordModel *()> &current_model, boost::intrusive_ptr<TreeItem>  target_item, const index_source &sibling_index);
	std::function<RecordModel *()>  _current_model;
//	boost::intrusive_ptr<TreeItem>  _target_sibling;
	// QModelIndex                     _current_index;
	boost::intrusive_ptr<TreeItem>	_host;
	index_source			_sibling_index;
};


#endif	// RECORDINDEX_HXX

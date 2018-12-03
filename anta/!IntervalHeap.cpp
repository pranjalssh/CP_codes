//performance: http://ideone.com/7LE9l3
namespace interval_heap {
typedef size_t Index;

template<typename It> void heap_up_min(It first, Index cnt) {
	while(cnt > 0) {
		Index parent = (cnt - 1) / 2;
		It parentit = first + parent * 2 + 0, cntit = first + cnt * 2 + 0;
		if(!(*cntit < *parentit)) break;
		swap(*parentit, *cntit);
		cnt = parent;
	}
}

template<typename It> void heap_up_max(It first, It last, Index cnt) {
	while(cnt > 0) {
		Index parent = (cnt - 1) / 2;
		It parentit = first + parent * 2 + 1, cntit = first + cnt * 2 + 1;
		if(cntit == last) -- cntit;
		if(!(*parentit < *cntit)) break;
		swap(*parentit, *cntit);
		cnt = parent;
	}
}

template<typename It> void push(It first, It last) {
	Index size = last - first - 1;
	It elem = last - 1;
	if(size % 2 != 0 && *elem < *(elem - 1))
		swap(*elem, *(elem - 1)), -- elem;
	if(size <= 1) return;
	Index node = size / 2, parent = (node - 1) / 2 * 2;
	if(*elem < *(first + parent))
		return heap_up_min(first, node);
	else if(*(first + parent + 1) < *elem)
		return heap_up_max(first, last, node);
}

template<typename It> inline It get_min(It first, It) { return first; }
template<typename It> inline It get_max(It first, It last) { return first + 1 == last ? first : first + 1; }

template<typename It> void heap_down_min(It first, It last) {
	Index size = last - first, cnt = 0;
	It cntit = first;
	while(1) {
		Index child = cnt * 2 + 1;
		if(size <= child * 2) break;
		It childit = first + child * 2 + 0;
		if(childit + 2 < last && *(childit + 2) < *childit)
			++ child, childit += 2;
		if(!(*childit < *cntit)) break;
		swap(*childit, *cntit);
		if(childit + 1 != last && *(childit + 1) < *childit)
			swap(*childit, *(childit + 1));
		cnt = child; cntit = childit;
	}
}

template<typename It> void heap_down_max(It first, It last) {
	Index size = last - first, cnt = 0;
	It cntit = first + 1;
	while(1) {
		Index child = cnt * 2 + 1;
		if(size <= child * 2) break;
		It childit = first + child * 2 + 1;
		if(childit < last - 2) {
			if(*childit < *(childit + 2))
				++ child, childit += 2;
		}else if(childit < last - 1) {
			if(*childit < *(childit + 1))
				++ child, childit += 1;
		}else if(childit == last)
			childit -= 1;
		if(!(*cntit < *childit)) break;
		swap(*childit, *cntit);
		if(childit + 1 != last && *childit < *(childit - 1))
			swap(*(childit - 1), *childit);
		cnt = child; cntit = childit;
	}
}

template<typename It> void pop_min(It first, It last) {
	Index size = last - first;
	if(size <= 1) return;
	Index lastnode = (size - 1) / 2;
	swap(*first, *(first + lastnode * 2 + 0));
	if(size % 2 == 0)
		swap(*(first + lastnode * 2 + 0), *(first + lastnode * 2 + 1));
	return heap_down_min(first, last - 1);
}

template<typename It> void pop_max(It first, It last) {
	Index size = last - first;
	if(size <= 1) return;
	Index lastnode = (size - 1) / 2;
	if(size % 2 == 0)
		swap(*(first + 1), *(first + lastnode * 2 + 1));
	else
		swap(*(first + 1), *(first + lastnode * 2 + 0));
	return heap_down_max(first, last - 1);
}

};

template<typename Val>
class double_ended_priority_queue {
	typedef vector<Val> Container;
	Container c;
public:
	double_ended_priority_queue(): c() { }

	bool empty() const { return c.empty(); }
	size_t size() const { return c.size(); }

	const Val &front() const {
		return *interval_heap::get_min(c.begin(), c.end());
	}
	const Val &back() const {
		return *interval_heap::get_max(c.begin(), c.end());
	}

	void push(const Val &val) {
		c.push_back(val);
		interval_heap::push(c.begin(), c.end());
	}
	void pop_front() {
		interval_heap::pop_min(c.begin(), c.end());
		c.pop_back();
	}
	void pop_back() {
		interval_heap::pop_max(c.begin(), c.end());
		c.pop_back();
	}
};

//memory efficientなbounded priority queue
template<typename Val>
class bounded_priority_queue {
	typedef size_t Index;
	Index m_capacity, m_bound, m_size;
	Val *c;
public:
	explicit bounded_priority_queue(Index bound):
		m_capacity(bound), m_bound(bound), m_size(0), c(new Val[bound]) { }
	~bounded_priority_queue() { delete[] c; }

	bool empty() const { return m_size == 0; }
	Index size() const { return m_size; }
	Index bound() const { return m_bound; }

	const Val &top() const {
		return *interval_heap::get_min(c, c + m_size);
	}
	bool push(const Val &val) {
		if(m_bound <= 0) return false;
		if(m_size < m_bound) {
			c[m_size ++] = val;
			interval_heap::push(c, c + m_size);
			return true;
		}else if(val < *interval_heap::get_max(c, c + m_size)) {
			interval_heap::pop_max(c, c + m_size);
			c[m_size - 1] = val;
			interval_heap::push(c, c + m_size);
			return true;
		}else return false;
	}
	void pop() {
		interval_heap::pop_min(c, c + (m_size --));
	}

	void narrow() {
		if(m_bound == 0) return;
		if(m_size == (m_bound --))
			interval_heap::pop_max(c, c + (m_size --));
	}
};

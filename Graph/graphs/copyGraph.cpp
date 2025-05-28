#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

// TODO: Make this graph generic
//       ... this won't just compile
//       straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		class iterator;
		struct value_type {
			N from;
			N to;
			E weight;
			auto operator==(value_type const& other) const -> bool {
				return (from == other.from and to == other.to and weight == other.weight);
			}
		};

		/* Temporary Test Print function */
		auto PrintGraphNodes() -> void;

		/*       CONSTRUCTORS        */
		graph();
		graph(std::initializer_list<N> il);
		template<typename InputIt>
		graph(InputIt first, InputIt last);
		graph(graph&& other) noexcept;
		auto operator=(graph&& other) noexcept -> graph&;
		graph(graph const& other);
		auto operator=(graph const& other) -> graph&;

		/*       MODIFIERS           */
		auto insert_node(N const& value) -> bool;
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool;
		auto replace_node(N const& old_data, N const& new_data) -> bool;
		auto merge_replace_node(N const& old_data, N const& new_data) -> void;
		auto erase_node(N const& value) -> bool;
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;
		auto erase_edge(iterator i) -> iterator;
		auto erase_edge(iterator i, iterator s) -> iterator;
		auto clear() noexcept -> void;

		// Your member functions go here

		/* Accessors */

		[[nodiscard]] auto is_node(N const& value) const -> bool;
		[[nodiscard]] auto empty() const -> bool;
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool;
		[[nodiscard]] auto nodes() const -> std::vector<N>;
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E>;
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N>;

		[[nodiscard]] auto operator==(graph const& other) const -> bool;
		//Iterator Access
		[[nodiscard]] auto begin() const -> iterator;
		[[nodiscard]] auto end() const -> iterator;


	private:
		friend class Iterator;
		struct Node {
			std::shared_ptr<N> src_;
			auto operator==(Node const& other) const -> bool {
				return (*src_.get() == *other.src_.get());
			}
		};
		struct Edge {
			std::shared_ptr<N> dst_;
			mutable std::set<E> weights_;
		};

		struct NodeComparator {
			using is_transparent = void;
			bool operator() (Node const& a, Node const& b) const
			{
				return *(a.src_.get()) < *(b.src_.get());
			}
			bool operator() (N const& b, Node const& a) const
			{
				return b < *a.src_.get();
			}
			bool operator() (Node const& a, N const& b) const
			{
				return *a.src_.get() < b;
			}
		} ;
		struct  EdgeComparator {

			using is_transparent = void;
			bool operator() (Edge const& a, Edge const& b) const
			{
				if (*a.dst_.get() == *b.dst_.get()) {
					a.weights_.merge(b.weights_);
				}
				return *a.dst_.get() < *b.dst_.get();
			}

			bool operator() (N const& dsti, Edge const& a) const
			{
				return dsti < *a.dst_.get();
			}

			bool operator() (Edge const& a, N const& dsti) const
			{
				return *a.dst_.get() < dsti;
			}

		};
		std::set<Node,NodeComparator> Nodes_;
		std::map<Node,std::set<Edge,EdgeComparator>,NodeComparator> Graph_; //Edges_
	};

	template<typename N, typename E>
	class graph<N,E>::iterator {

		using outer_iter = typename std::map<Node,std::set<Edge,EdgeComparator>,NodeComparator>::const_iterator;
		using middle_iter = typename std::set<Edge,EdgeComparator>::const_iterator;
		using inner_iter = typename std::set<E>::const_iterator;

		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void; //whats this shit for
			using difference_type = std::ptrdiff_t; //this too
			using iterator_category = std::bidirectional_iterator_tag; //and this
			iterator() = default;

			auto operator*() const -> reference {
				auto Values = value_type{};
				Values.from = *outer_->first.src_.get();
				Values.to = *middle_->dst_.get();
				Values.weight = *inner_;
				return Values;
			}

			auto operator++() -> iterator& {
				/*
				inner_++;
				if (inner_ == middle_->weights_.end()) {
					//std::cout << "This" << std::endl;
					middle_++;
					inner_ = middle_->weights_.begin();
					if (middle_ == std::prev(outer_->second.end())) {
						outer_++;
						if (outer_ == std::prev(pointee_->end())) {
							return *this;
						}//if outer is not at its "end"
						outer_++;
						middle_ = outer_->second.begin();
						inner_ = middle_->weights_.begin();
					}//if middle is not at its "end"
				}//if inner is not at its end
				return *this;
			}
			*/

				if (inner_ != middle_->weights_.end()){
					inner_++;
					if (inner_ != middle_->weights_.end()){
						return *this;
					}
				}

				if (middle_ != outer_->second.end()) {
					middle_++;
					if (middle_ != outer_->second.end()) {
						inner_ = middle_->weights_.begin();
						return *this;
					} //if it does equal to the end
				}

				if (outer_ != pointee_->end()) {
					outer_++;
					if (outer_ != pointee_->end()) {
						middle_ = outer_->second.begin();
						inner_ = middle_->weights_.begin(); //
						return *this;
					}
				}
				outer_--;
				middle_--;
				return *this;
			}

			auto operator++(int) -> iterator {
				auto temp = *this;
				++*this;
				return temp;
			} //post
			auto operator--() -> iterator& {
				if (inner_ == middle_->weights_.begin()) {
					if (middle_ == outer_->second.begin()) {
						if (outer_ == pointee_->begin()) {
							//Our beginning is literally the beginning
							return *this;
						}
						outer_--;
						middle_ = std::prev(outer_->second.end());
						inner_ = std::prev(middle_->weights_.end());
						return *this;
					}
					middle_--;
					inner_ = std::prev(middle_->weights_.end());
					return *this;
				}
				inner_--;
				return *this;
			}
			auto operator--(int) -> iterator {
				auto temp = *this;
				--*this;
				return temp;
			}

			auto operator==(iterator const& other) const -> bool {
				if(pointee_ == other.pointee_) {
					if (inner_ == other.inner_
					and middle_ == other.middle_
					and outer_ == other.outer_)
					return true;
				}
				return false;
			}

		private:
			friend class graph;
			explicit iterator(std::map<Node,std::set<Edge,EdgeComparator>,NodeComparator>  const&pointee
			, outer_iter outer
			, middle_iter middle
			, inner_iter inner)
			: pointee_(&pointee)
			  , outer_(outer), middle_(middle), inner_(inner) {}

			mutable std::map<Node,std::set<Edge,EdgeComparator>,NodeComparator> const *pointee_ = nullptr;
			mutable outer_iter outer_;
			mutable middle_iter middle_;
			mutable inner_iter inner_;
	};

	template<typename N, typename E>
	auto graph<N,E>::PrintGraphNodes() -> void
	{

		for (auto &i : Graph_) {
			std::cout << "SRC -> " << *i.first.src_.get() << ", " << "Memory Address  = " << static_cast<void*>(i.first.src_.get()) << std::endl;

			for (auto &j : i.second) {
				std::cout << "DST of -> "<< *i.first.src_.get() << ", DST -> " << *j.dst_.get() << ", " << "Memory Address = " << static_cast<void*>(j.dst_.get()) << ", WEIGHTS :";
				for (auto &k : j.weights_) {
					std::cout << " " << k << ",";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		std::cout << "Calculating the number of shared_ptr each value has" << std::endl;
		for (auto &i : Graph_) {
			std::cout << "Value : " << *i.first.src_.get() << ",  Shared Pointer Count :" << i.first.src_.use_count() << std::endl;
		}
	}

	template<typename N, typename E>
	graph<N,E>::graph(){
		Graph_ = std::map<Node,std::set<Edge,EdgeComparator>,NodeComparator> ();
		Nodes_ = std::set<Node,NodeComparator>();
	}

	template<typename N, typename E>
	template<typename InputIt>
	graph<N,E>::graph(InputIt first, InputIt last) {
		graph();
		while (first != last) {
			insert_node(*first);
			++first;
		}
	}

	template<typename N, typename E>
	graph<N,E>::graph(std::initializer_list<N> il) {
		graph();
		for (auto &x : il) {
			insert_node(x);
		}
	}

	template<typename N, typename E>
	graph<N,E>::graph(graph&& other) noexcept {
		graph();
		Graph_.swap(other.Graph_);
		Nodes_.swap(other.Nodes_);
	}

	template<typename N, typename E>
	auto graph<N,E>::operator=(graph&& other) noexcept -> graph& {
		clear();
		Graph_.swap(other.Graph_);
		Nodes_.swap(other.Nodes_);
		return *this;
	}

	template<typename N, typename E>
	graph<N,E>::graph(graph const& other) {
		for(auto &x : other.Nodes_) {
			insert_node(*x.src_.get());
		}
		for (auto y = other.begin(); y != other.end(); ++y) {
			auto a = *y;
			insert_edge(a.from, a.to, a.weight);
		}
	}
	template<typename N, typename E>
	auto graph<N,E>::operator=(graph const& other) -> graph& {

		clear();
		for(auto &x : other.Nodes_) {
			insert_node(*x.src_.get()); //n steps
		}
		for (auto y = other.begin(); y != other.end(); ++y) {
			auto a = *y;
			insert_edge(a.from, a.to, a.weight); //e steps
		}
		return *this;
	}


	/* Modifiers */

	template<typename N, typename E>
	auto graph<N,E>::insert_node(N const& value) -> bool
	{
		// If node doesnt exist we add it
		if (!is_node(value)) {
			auto newNode = Node{};
			newNode.src_ = std::make_shared<N>(value);
			Nodes_.insert(std::move(newNode));

			return true;
		}
		return false;
	}


	template<typename N, typename E>
	auto graph<N,E>::insert_edge(N const& src, N const& dst, E const& weight) -> bool
	{
		//We can only insert edges if and only if both nodes given are in the
		// Graph_ map
		if (!is_node(src) or !is_node(dst)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
		} else {

			if (Graph_.find(*Nodes_.find(src)->src_.get()) == Graph_.end()) {
				auto in = Node{};
				in.src_ = Nodes_.find(src)->src_;
				Graph_[std::move(in)];
			}

			auto Fsrc = Graph_.find(src);
			auto it = Fsrc->second.find(dst);
			auto end = Fsrc->second.end();
			if (it == end) {
				auto NewEdge = Edge{};
				NewEdge.dst_ = Nodes_.find(dst)->src_;
				NewEdge.weights_.insert(weight);
				Graph_.find(src)->second.insert(std::move(NewEdge));
				return true;
			} else {
				if (it->weights_.find(weight) == it->weights_.end()) {
					it->weights_.insert(weight);
					return true;
				} else {
					return false;
				}
			}
		}
	}
	template<typename N, typename E>
	auto graph<N,E>::replace_node(N const& old_data, N const& new_data) -> bool
	{
		if (!is_node(old_data)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
		}
		if (is_node(new_data)) {
			return false;
		}
		auto NewSPTR = std::make_shared<N>(new_data);
		for (auto x = Graph_.begin(); x != Graph_.end(); ++x) {
			if (is_connected(*x->first.src_.get(),old_data)) {
				auto EditEdge = x->second.extract(x->second.find(old_data));
				EditEdge.value().dst_ = NewSPTR;
				x->second.insert(std::move(EditEdge));
			}
		}
		auto OldNode = Nodes_.extract(Nodes_.find(old_data));
		auto OldPair = Graph_.extract(Graph_.find(old_data));
		OldPair.key().src_ = NewSPTR;
		OldNode.value().src_ = NewSPTR;
		Nodes_.insert(std::move(OldNode));
		Graph_.insert(std::move(OldPair));
		return true;
	}

	template<typename N, typename E>
	auto graph<N,E>::merge_replace_node(N const& old_data, N const& new_data) -> void
	{
		if(!is_node(old_data) or !is_node(new_data)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
		}
		auto NewSPTR = Graph_.find(new_data)->first.src_;
		for (auto x = Graph_.begin(); x != Graph_.end(); ++x) {
			if (is_connected(*x->first.src_.get(),old_data)) {
				auto EditEdge = x->second.extract(x->second.find(old_data));
				EditEdge.value().dst_ = NewSPTR;
				x->second.insert(std::move(EditEdge));
			}
		}
		auto OldPair = Graph_.extract(Graph_.find(old_data));
		auto OldNode = Nodes_.extract(Nodes_.find(old_data));
		OldNode.value().src_ = NewSPTR;
		OldPair.key().src_ = NewSPTR;
		Graph_.find(new_data)->second.merge(std::move(OldPair.mapped()));
	}

	template<typename N, typename E>
	auto graph<N,E>::erase_node(N const& value) -> bool {
		if (!is_node(value)) {
			return false;
		}
		for (auto x = Graph_.begin(); x != Graph_.end(); ++x) {
			if (is_connected(*x->first.src_.get(),value)) {
				x->second.erase(x->second.find(value));
			}
		}
		Nodes_.erase(Nodes_.find(value));
		Graph_.erase(Graph_.find(value));
		return true;
	}

	template<typename N, typename E>
	auto graph<N,E>::erase_edge(N const& src, N const& dst, E const& weight) -> bool {
		if (!is_node(src) or !is_node(dst)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
		}
		//check if dst is an edge of src first
		if (!is_connected(src,dst)) {
			return false;
		}
		if (Graph_.find(src) == Graph_.end()) {
			return false;
		}

		if (Graph_.find(src)->second.find(dst)->weights_.find(weight) ==
			Graph_.find(src)->second.find(dst)->weights_.end()) {
			return false;
		} else {
			Graph_.find(src)->second.find(dst)->weights_.erase(Graph_.find(src)->second.find(dst)->weights_.find(weight));
			if (Graph_.find(src)->second.find(dst)->weights_.size() == 0) {
				Graph_.find(src)->second.erase((Graph_.find(src)->second.find(dst)));
			}
			if (Graph_.find(src)->second.size() == 0) {
				Graph_.erase(Graph_.find(src));
			}
			return true;
		}
	}

	template<typename N, typename E>
	auto graph<N,E>::erase_edge(iterator i) -> iterator {
		auto tmp = i;
		++tmp;
		tmp = tmp == end() ? end() : tmp;
		auto outer = Graph_.erase(i.outer_, i.outer_);
		auto middle = outer->second.erase(i.middle_, i.middle_);
		auto inner = middle->weights_.erase(i.inner_, i.inner_);

		middle->weights_.erase(inner);
		if (middle->weights_.size() == 0) {
			outer->second.erase(middle);
			if (outer->second.size() == 0) {
				Graph_.erase(outer);
			}
		}
		return tmp;
	}
	template<typename N, typename E>
	auto graph<N,E>::erase_edge(iterator i, iterator s) -> iterator {
		//if i and s are equal
		if (i == s) {
			return i;
		}
		while(i != s ) {
			i = erase_edge(i);
			if (i == end()) {
				break;
			}
		}
		return i;
	}

	template<typename N, typename E>
	auto graph<N,E>::clear() noexcept -> void {
		Graph_.clear();
		Nodes_.clear();
	}

	/* Accessors */
	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::is_node(N const& value) const -> bool {
		if (Nodes_.find(value) == Nodes_.end()) {
			return false;
		}
		return true;
	}
	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::empty() const -> bool {
		if (Nodes_.size() == 0)
		{
			return true;
		}
		return false;
	 }

	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::is_connected(N const& src, N const& dst) const -> bool {
		if (!is_node(src) or !is_node(dst)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
		} else {
			if (Graph_.find(src)->second.find(dst) == Graph_.find(src)->second.end()) {
				return false;
			}
			return true;
		}
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::nodes() const -> std::vector<N> {
		auto Nodes = std::vector<N>();
		transform(Nodes_.begin(), Nodes_.end(), back_inserter(Nodes),
				[](auto &p) { return *p.src_.get();} );
		return Nodes;
	}


	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::weights(N const& src, N const& dst) const -> std::vector<E> {
		auto Weights = std::vector<E>();
		transform(Graph_.find(src)->second.find(dst)->weights_.begin(), Graph_.find(src)->second.find(dst)->weights_.end()
				, back_inserter(Weights), [](auto &p) { return p;});
		return Weights;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::connections(N const& src) const -> std::vector<N> {
		auto Edges = std::vector<N>();
		transform(Graph_.find(src)->second.begin(), Graph_.find(src)->second.end()
				, back_inserter(Edges), [](auto &p) {return *p.dst_.get();});
		return Edges;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::find(N const& src, N const& dst, E const& weight) -> iterator {
		//given src,dst,weight I return an iterator in log(n) + log(e)
		if (empty()) {
			return end();
		}
		auto outer = Graph_.find(src);
		if (outer == Graph_.end()) {
			return end();
		}
		auto middle = outer->second.find(dst);
		if (middle ==  outer->second.end()) {
			return end();
		}
		auto inner = middle->weights_.find(weight);
		if (inner == middle->weights_.end()) {
			return end();
		}
		return iterator(Graph_,outer, middle, inner);
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::operator==(graph const& other) const -> bool {
		if (std::equal(Nodes_.begin(), Nodes_.end(), other.Nodes_.begin(), other.Nodes_.end())) {
			if (std::equal(begin(), end(), other.begin(), other.end())) {
				return true;
			}
		}
		return false;
	}

	//Iterators
	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::begin() const -> iterator {
		//If there is no edges
		if (Graph_.size() > 0) {
			return iterator(Graph_
				, Graph_.begin()
				, Graph_.begin()->second.begin()
				, Graph_.begin()->second.begin()->weights_.begin()
			);
		}
		//empty case
		auto inner = decltype(Graph_.begin()->second.begin()->weights_.begin()){};
		auto middle = decltype(Graph_.begin()->second.begin()){};
		auto outer = Graph_.begin();
		//unique_ptr that points to this new iterator object
		//Iterator should have private metho
		return iterator(Graph_,outer,middle,inner);
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N,E>::end() const -> iterator {
		if (Graph_.size() == 0) {
			//empty case scenario
			return iterator(
				Graph_
				, Graph_.end()
				, decltype(Graph_.end()->second.end()){}
				, decltype(Graph_.end()->second.end()->weights_.end()){}
			);
		}
		return iterator(Graph_
			, std::prev(Graph_.end())
			, std::prev(std::prev(Graph_.end())->second.end()) //std::prev(middle.end())
			, std::prev(std::prev(Graph_.end())->second.end())->weights_.end()
		);
	}
} // namespace gdwg

#endif // GDWG_GRAPH_HPP

#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace gdwg {
	template<typename N, typename E>
	class graph {1
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
		//[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N>;

		[[nodiscard]] auto operator==(graph const& other) const -> bool;
		//Iterator Access
		[[nodiscard]] auto begin() const -> iterator;
		[[nodiscard]] auto end() const -> iterator;


	private:
		struct Node {
			std::shared_ptr<N> src_;
			auto operator==(Node const& other) const -> bool {
				return (*src_.get() == *other.src_.get());
			}
		};

		struct Edges {
			mutable std::set<Edge, EdgeComparator> Edges_;
		}

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
		std::map<Node,Edges,NodeComparator> Graph_; //Edges_
		std::shared_ptr<graph<N,E>*> GraphAddress_;
	};

    //Dw about constructors

    //New modifiers
    auto graph
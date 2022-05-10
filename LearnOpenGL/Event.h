#pragma once

#include <functional>
#include <map>
#include <vector>

#include <iostream>

namespace dap {
	template <class... Args>
	class Event {
	public:
		~Event () {
			Clear ();
		}

		void Clear () {
			for ( auto &p : subscribers ) {
				for ( auto *sub : p.second ) {
					delete sub;
				}
			}
			subscribers.clear ();
		}

		void RemoveBinder ( const void* binder ) {
			if ( !subscribers.count ( binder ) )
				return;
			for ( auto* sub : subscribers[binder] ) {
				delete sub;
			}
			subscribers.erase ( binder );
		}

		template <class T>
		std::function<void (Args...)>* Subscribe ( void (T::*fp) (Args... args), T* binder ) {
			std::function<void ( Args... )>* function = new std::function<void ( Args... )> ( [fp, binder](Args... args) {
				std::invoke ( fp, binder, args... );
			} );
			if ( !subscribers.count ( binder ) )
				subscribers.insert ( std::pair<void*, std::vector<const std::function<void ( Args... )>*>> ( binder, {} ) );
			subscribers[binder].push_back ( function );
			return function;
		}

		template <class T>
		void Unsubscribe ( const void* binder, std::function<void (Args...)>* subscriber ) {
			if ( !subscribers.count ( binder ) )
				return;
			if ( !std::count ( subscribers[binder].begin (), subscribers[binder].end (), subscriber ) )
				return;
			subscribers[binder].erase ( std::remove ( subscribers[binder].begin (), subscribers[binder].end (), subscriber ), subscribers[binder].end () );
			delete subscriber;
			subscriber = NULL;
		}

		void Invoke ( Args... args ) {
			for ( const auto &subBindingPair : subscribers ) {
				for ( const auto *subscriber : subBindingPair.second ) {
					std::invoke ( *subscriber, args... );
				}
			}
		}
	private:
		std::map<const void*, std::vector<const std::function<void ( Args... )>*>> subscribers = {};
	};
}

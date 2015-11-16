#ifndef AA_SIGNAL_H
#define AA_SIGNAL_H

#include <map>
#include <functional>
#include <memory>

namespace aa
{
	template<typename Result, typename... Args>
	class connection;

	template<typename Result, typename... Args>
	class signal;

	template<typename Result, typename... Args>
	class connection<Result(Args...)>;

	template<typename Result, typename... Args>
	class signal<Result(Args...)>
	{
		private:
			std::shared_ptr<std::map<unsigned int, std::function<Result(Args...)>>> funcs;
			unsigned int next_id;

		public:
			signal();
			signal(const signal<Result(Args...)> &other) = delete;
			signal(signal<Result(Args...)> &&other) = default;

			signal &operator=(const signal<Result(Args...)> &other) = delete;
			signal<Result(Args...)> &operator=(signal<Result(Args...)> &&other) = default;

			connection<Result(Args...)> connect(const std::function<Result(Args...)> &func);
			Result operator()(Args... args);
			bool empty();
	};

	template<typename... Args>
	class signal<void(Args...)>
	{
		private:
			std::shared_ptr<std::map<unsigned int, std::function<void(Args...)>>> funcs;
			unsigned int next_id;

		public:
			signal();
			signal(const signal<void(Args...)> &other) = delete;
			signal(signal<void(Args...)> &&other) = default;

			signal &operator=(const signal<void(Args...)> &other) = delete;
			signal<void(Args...)> &operator=(signal<void(Args...)> &&other) = default;

			connection<void(Args...)> connect(const std::function<void(Args...)> &func);
			void operator()(Args... args);
			bool empty();
	};


	template<typename Result, typename... Args>
	class connection<Result(Args...)>
	{
		friend class signal<Result(Args...)>;

		private:
			std::weak_ptr<std::map<unsigned int, std::function<Result(Args...)>>> connected_signal;
			unsigned int connection_id;

			connection(std::weak_ptr<std::map<unsigned int, std::function<Result(Args...)>>> signal, unsigned int id);

		public:
			connection(const connection<Result(Args...)> &other) = delete;
			connection(connection<Result(Args...)> &&other) = default;

			connection &operator=(const signal<Result(Args...)> &other) = delete;
			connection<Result(Args...)> &operator=(connection<Result(Args...)> &&other) = default;

			void disconnect();
	};

	template<typename Result, typename... Args>
	signal<Result(Args...)>::signal()
		: next_id(0),
		  funcs(std::make_shared<std::map<unsigned int, std::function<void(Args...)>>>())
	{
	}

	template<typename Result, typename... Args>
	connection<Result(Args...)> signal<Result(Args...)>::connect(const std::function<Result(Args...)> &func)
	{
		funcs.insert({next_id, func});
		connection<Result(Args...)> conn(this, next_id);
		next_id++;
		return conn;
	}

	template<typename Result, typename... Args>
	Result signal<Result(Args...)>::operator()(Args... args)
	{
		Result res;
		for(auto &p : funcs)
		{
			res = p.second(args...);
		}
		return res;
	}

	template<typename Result, typename... Args>
	bool signal<Result(Args...)>::empty()
	{
		return funcs.empty();
	}

	template<typename... Args>
	signal<void(Args...)>::signal()
		: next_id(0),
		  funcs(std::make_shared<std::map<unsigned int, std::function<void(Args...)>>>())
	{
	}

	template<typename... Args>
	connection<void(Args...)> signal<void(Args...)>::connect(const std::function<void(Args...)> &func)
	{
		funcs->insert({next_id, func});
		connection<void(Args...)> conn(funcs, next_id);
		next_id++;
		return conn;
	}

	template<typename... Args>
	void signal<void(Args...)>::operator()(Args... args)
	{
		for(auto &p : *funcs)
		{
			p.second(args...);
		}
	}

	template<typename... Args>
	bool signal<void(Args...)>::empty()
	{
		return funcs->empty();
	}

	template<typename Result, typename... Args>
	connection<Result(Args...)>::connection(std::weak_ptr<std::map<unsigned int, std::function<Result(Args...)>>> signal, unsigned int id)
		: connected_signal(signal),
		  connection_id(id)
	{
	}

	template<typename Result, typename... Args>
	void connection<Result(Args...)>::disconnect()
	{
		if(auto shared = connected_signal.lock())
		{
			shared->erase(connection_id);
		}
	}
}

#endif AA_SIGNAL_H

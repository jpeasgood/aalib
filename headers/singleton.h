#ifndef AA_SINGLETON_H
#define AA_SINGLETON_H

namespace aa
{
	template<typename T>
	class singleton
	{
		public:
			static T *get_instance()
			{
				static T instance;
				return &instance;
			}

		protected:
			singleton() = default;
			~singleton() = default;

		private:
			singleton(const singleton &other) = delete;
			singleton(singleton &&other) = delete;

			singleton &operator=(const singleton &other) = delete;
			singleton &operator=(singleton &&other) = delete;
	};
}

#endif

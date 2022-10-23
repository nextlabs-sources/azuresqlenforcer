#ifndef CYCLE_TASK_MANAGER
#define CYCLE_TASK_MANAGER
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
class CycleTaskManager
{
public:
	static CycleTaskManager* GetInstance();
	CycleTaskManager();
	void PushTask(const std::function<void(void)>& f, size_t deltaSeconds);
	boost::shared_ptr<boost::asio::deadline_timer> PushDelayTask(const std::function<void(void)>& f, size_t deltaSeconds);
private:
	class Task : public boost::enable_shared_from_this<Task>
	{
	public:
		Task(const std::function<void(void)>& f) : m_f(f) {}
		void Call(const boost::system::error_code& error, boost::shared_ptr<boost::asio::deadline_timer> timer, size_t deltaSeconds)
		{
			m_f();
			timer->expires_from_now(boost::posix_time::seconds(deltaSeconds));
			timer->async_wait(boost::bind(&Task::Call, shared_from_this(), boost::placeholders::_1, timer, deltaSeconds));
		}
		void CallOnce(const boost::system::error_code& error, boost::shared_ptr<boost::asio::deadline_timer> timer, size_t deltaSeconds)
		{
			if (!error)
				m_f();
		}
	private:
		std::function<void(void)> m_f;
	};
private:
	static CycleTaskManager* ins;
	boost::asio::io_service* m_io;
	boost::asio::io_service::work* worker;
};
#endif // !CYCLE_TASK_MANAGER

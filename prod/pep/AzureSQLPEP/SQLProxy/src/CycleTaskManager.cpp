#include "CycleTaskManager.h"

CycleTaskManager* CycleTaskManager::ins = nullptr;

CycleTaskManager* CycleTaskManager::GetInstance()
{
	if (nullptr == ins)
		ins = new CycleTaskManager;
	return ins;
}

CycleTaskManager::CycleTaskManager()
{
	m_io = new boost::asio::io_service;
	worker = new boost::asio::io_service::work(*m_io);
	boost::thread t(boost::bind(&boost::asio::io_service::run, m_io));
}

void CycleTaskManager::PushTask(const std::function<void(void)>& f, size_t deltaSeconds)
{
	boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*m_io));
	timer->expires_from_now(boost::posix_time::seconds(deltaSeconds));
	boost::shared_ptr<Task> task(new Task(f));
	timer->async_wait(boost::bind(&Task::Call, task, boost::placeholders::_1, timer, deltaSeconds));
}

boost::shared_ptr<boost::asio::deadline_timer> CycleTaskManager::PushDelayTask(const std::function<void(void)>& f, size_t deltaSeconds)
{
	boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*m_io));
	timer->expires_from_now(boost::posix_time::seconds(deltaSeconds));
	boost::shared_ptr<Task> task(new Task(f));
	timer->async_wait(boost::bind(&Task::CallOnce, task, boost::placeholders::_1, timer, deltaSeconds));
	return timer;
}


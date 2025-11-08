#pragma once

template<typename... Args>
class FDelegate
{
public:
	void operator()(Args... args)
	{
		auto snapShot = m_Functions;

		for (auto& pair :snapShot)
		{
			if (alive(pair.first))
				pair.second(args...);
		}
		clear();
	}

	_uint add(const std::function<void(Args...)>& f)
	{
		m_Functions.emplace(m_iId, f);

		return m_iId++;
	}

	void remove(_uint id)
	{
		m_RemoveIDs.push_back(id);
	}

	_bool empty() { return m_Functions.empty(); }
	_uint size() { return static_cast<_uint>(m_Functions.size()); }

private:
	_bool alive(_uint id)
	{
		for (auto& pair : m_Functions)
		{
			if (pair.first == id)
				return true;
		}
		return false;
	}
	void clear()
	{
		if (m_RemoveIDs.empty())
			return;

		m_Functions.erase(std::remove_if(m_Functions.begin(), m_Functions.end(),
			[this](std::pair<_uint, std::function<void(Args...)>>& slot) {
				return std::find(m_RemoveIDs.begin(), m_RemoveIDs.end(), slot.first)
					!= m_RemoveIDs.end();
			}), m_Functions.end());
	}


	_uint m_iId{};
	std::vector<std::pair<_uint,std::function<void(Args...)>>> m_Functions;
	std::vector<_uint> m_RemoveIDs;

};
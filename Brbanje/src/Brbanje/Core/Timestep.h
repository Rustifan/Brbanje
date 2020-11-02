#pragma once

namespace Brbanje
{
	class Timestep
	{
	private:
		float m_Time;
	public:
		Timestep(float time = 0) : m_Time(time)
		{

		}
		float GetMiliseconds()const { return m_Time * 1000; }
		const float& GetSeconds()const { return m_Time; }
		operator float() const { return m_Time; }
	};
}
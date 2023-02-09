#pragma once



// Another alternative is use dynamic storage
template<class T, class TArgs = void*, int hBuffer = 256>
struct Event
{
	// Type of function pointer that holds the handler
	// TODO: add suport for EventArgs
	typedef void(*EventHandler)(T source, TArgs& args);

	size_t Count() const
	{
		int old_ptr = m_ptr;
		size_t c = 0;
		while (old_ptr-- > 0)
			c++;

		return c;
	}

	Event& operator +=(const EventHandler handler)
	{
		if (m_ptr < hBuffer)
			m_handlers[m_ptr++] = handler;
		return *this;
	}

	Event& operator -=(const EventHandler handler)
	{
		int old_ptr = m_ptr;
		bool skipFix = true;

		// Removes the element ( if find )
		while (old_ptr-- > 0)
		{
			if (m_handlers[old_ptr] == handler)
				skipFix = m_handlers[old_ptr] = 0;
		}

		// there is no reasson to fix holes if there is any hole
		if (skipFix)
			return *this;

		// Fix the holes in the list
		while (old_ptr++ < m_ptr)
		{
			if (m_handlers[old_ptr] == 0)
				if (old_ptr < m_ptr - 1)
					if (m_handlers[old_ptr + 1] != 0)
					{
						m_handlers[old_ptr] = m_handlers[old_ptr + 1];
						m_handlers[old_ptr + 1] = 0;
					}
		}
		m_ptr--;
		return *this;
	}

	void Invoke(T source, TArgs& args)
	{
		int old_ptr = m_ptr;
		while (old_ptr-- > 0)
			if (m_handlers[old_ptr] != 0)
				m_handlers[old_ptr](source, args);

	}
private:
	EventHandler* m_handlers = new EventHandler[hBuffer];
	int m_ptr = 0;
};

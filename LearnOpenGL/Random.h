#pragma once

namespace dap {
	class Random {
	private:
		static const bool WAS_SEEDED;

	public:
		static float Range ( float min, float max );
		static int Range ( int min, int max );
		static bool Seed ( unsigned int seed );

	private:
		Random ();
	};
}

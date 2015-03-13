#include <LSM303.h>

class MyLSM303 : public LSM303 {
    public:
	float yaw(void);
	float pitch(void);
	float roll(void);
	template <typename T> float vertical_heading(vector<T> from);
    private:
};

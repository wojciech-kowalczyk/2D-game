#ifndef XML_SETTINGS_H
#define XML_SETTINGS_H

#include "Base.h"
#include "config.h"
#include "Others/Status.h"
#include "Math/Point.h"

namespace xml {
	class Settings : public Base {
	public:
		/// \warning Can generate error
		void load();
		void save() const override;
		void setDefault() override;
		
		struct Window {
			int frameRateLimit {120};
			Status verticalSync {On};
			int antialiasing {8};
			Status maximize {On};
			Size<int> size {1500, 900};
		} window;
		
		struct Camera {
			int moveSpeed {800};
			Size<int> margin {300, 200};
		} camera;

		static constexpr char const *PATH {SETTINGS_FILE_PATH};
	};
}

#endif // XML_SETTINGS_H

#include <openvr.h>
#include <vector>
#include <iostream>
#include <thread>

#include "openvr_string_std.h"

using namespace vr;
using namespace std;
using namespace openvr_string;

class TrackerMonitor
{
	uint32_t m_index;
	vector<TrackedDevicePose_t> snapshot;
	vector<TrackedDevicePose_t> samples;

public:
	TrackerMonitor(uint32_t tracker_index) : m_index(tracker_index)
	{
		snapshot.resize(tracker_index + 1);
	}
	void TakeSample()
	{
		VRSystem()->GetDeviceToAbsoluteTrackingPose(TrackingUniverseStanding, 0, snapshot.data(), (uint32_t)snapshot.size());
		const TrackedDevicePose_t &pose = snapshot[m_index];
		samples.push_back(pose);
	}
	void DumpSamples()
	{
		for(auto sample : samples)
		{
			cout << to_string(sample.vAngularVelocity);
		}
	}
};

int main()
{
	EVRInitError err;
	IVRSystem *sys = VR_Init(&err, VRApplication_Other, "");
	if (sys)
	{
		uint32_t indices[k_unMaxTrackedDeviceCount];
		uint32_t num_trackers = 0;

		bool found_tracker = false;
		while (num_trackers == 0)
		{
			num_trackers = sys->GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass_GenericTracker, indices, k_unMaxTrackedDeviceCount);
			if (num_trackers == 0)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		

		if (num_trackers > 0)

		{
			while (1)
			{
				TrackerMonitor m(indices[0]);
				printf("press enter to start sampling");
				getchar();

				float sample_duration = 1.0f;

				float samples_per_second = 10;
				int num_samples = sample_duration * samples_per_second;
				int delay_per_sample_ms = 1.0f / samples_per_second * 1000.0f;

				for (int i = 0; i < num_samples; i++)
				{
					m.TakeSample();
					std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_sample_ms));
				}

				m.DumpSamples();
			}
		}
	}
}


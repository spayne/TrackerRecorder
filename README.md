# TrackerRecorder
Samples tracker poses (TrackedDevicePose_t) at a high frequency and dumps them out.  

The idea is that you can't just log them out using printf because printf inserts it's own timing.  So record a bunch
of device pose frames with high precision and then dump them out at the end.



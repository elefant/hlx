PREREQUISITE
------------
1) Run preinstall.sh to install some basic tools
2) Run install_ghost.sh if you don't know what ghost is
3) Run build_ffmpeg.sh if you have never install ffmpeg

BUILD
-----
1) $ cmake -DCMAKE_INSTALL_PREFIX=/opt   # Configurate make files
2) $ make      # Build all projects
3) $ make doc  # Generate doxygen documents

INSTALL
----
$ sudo make install

LAUNCH
-----
$ bin/StreamingController

DBUS Interfaces
-----
$ qdbus com.disternet.stream /controller start
true

$ qdbus com.disternet.stream /controller addStreaming some_not_existed_media
-1 # invalid channel id to indicate failed to add stream 

$ qdbus com.disternet.streaming /controller addStreaming /opt/disternet/media/Trailer.wmv
1 # transient channel id: 1 

$ qdbus com.disternet.streaming /controller addStreaming /opt/disternet/media/Trailer.wmv
2

$ qdbus com.disternet.streaming /controller addStreaming /opt/disternet/media/Trailer.wmv
3

$ qdbus com.disternet.streaming /controller getTotalStreaming
3

$ qdbus com.disternet.streaming /controller getStreamingList
1:/opt/disternet/media/Trailer.wmv,2:/opt/disternet/media/Trailer.wmv,3:/opt/disternet/media/Trailer.wmv, # ([transient channel id]:[source path],)*

$ qdbus com.disternet.streaming /controller getHttpLiveUri 1
live/1/playlist.m3u8

$ qdbus com.disternet.streaming /controller getRtmpUri 1
ch1

$ qdbus com.disternet.streaming /controller stopStreaming 4
false

$ qdbus com.disternet.streaming /controller stopStreaming 2
true

$ qdbus com.disternet.streaming /controller getStreamingList
1:/opt/disternet/media/Trailer.wmv,3:/opt/disternet/media/Trailer.wmv,

$ qdbus com.disternet.streaming /controller quit

Configuration
-----
You can configurate the server by providing streamingSetting.XML at the same path as StreamingController.
Following is the default one.

<?xml version="1.0" encoding="UTF-8"?>
<disternet>
  <serversetting listen_port="2935" max_threads="20" connection_check_ms="60000"/>
  <livehttp web_root="/opt/disternet/streaming/" segment_exe="/opt/disternet/bin/Segmenter" index_constraint="10" segment_length="3" bitrate_update_range="5"/>
  <rtmp listen_port="2936" command_port="2937"/>
  <transcoder target_start_port="7100" video_width="320" video_height="240" video_frame="20" video_bitrates="96000:256000"/>
  <segmenter template="{segmenter} -p {livehttp_target_port} -b {bitrate} -d {export_folder} -n playlist -i {index_constraint} -l {segment_length} -m {file_prefix}"/>
</disternet>

There is still some work to do on the configuration. Here explains the meaning of each congiguration.

---------------------
--- serversetting ---
---------------------
  listen_port: The listening port of the server for whatever the streaming protocol.
  
  max_threads: The maximun threads can be created to handle the clients. This is NOT equivallent to 
               the maximum number of clients we can handle at a time.
  
  connection_check_ms: The period we do the resource recycling

----------------
--- livehttp ---
----------------
  web_root: The working directory of HTTP Live used to place the generated segment files
  
  segment_exe: Not used right now.
  
  index_constraint: The maximum number of segment for a streaming source
  
  segment_length: The length in second for each segment
  
  bitrate_update_range: The minimum interval to update the bitrate
  
------------
--- rtmp ---
------------
  All attributes are not used right now.
  
------------------
--- transcoder ---
------------------
  target_start_port: The base port to generate other ports for all usages.

-----------------
--- segmenter ---
-----------------
  All attributes are not used right now.
  




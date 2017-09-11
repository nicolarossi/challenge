
# Documentation

In the ./doc/ folder is the doc used.

Iso standard :

* https://forums.xilinx.com/xlnx/attachments/xlnx/DSPTOOL/15095/1/iso13818-1.pdf

some introductive link not archived :

* http://www.erg.abdn.ac.uk/future-net/digital-video/mpeg2-trans.html
* http://www.img.lx.it.pt/~fp/cav/Additional_material/MPEG2_overview.pdf


# Usage

From Linux command line :


* to clean the environment

``` make clean ```

* to compile

``` make compile ```

* to testing 

``` make test ```

If the ```elephants.ts``` test case is not present it will be downloaded, will requires ```wget``` .

To automate testing during development was used the command line program ```file``` but in the acceptance test it should be used the ```ffplay``` command  line tool.

The ```file``` tool recognises only the audio stream format ( MPEG ADTS, AAC, v4 LC, 48 kHz, stereo) but using ```ffplay``` the video and audio stream are viewed/played correctly .

* Output

The TS parser will splice the file ```elephants.ts``` present in the ./ folder , and it is going to create separate file for every stream named ```./out/stream_PID ``` where PID is the Program ID in the TS packet .



# TODO 

* Read the PAT , to avoid writing the stream for PID 0 and 32 and to recognize the stream associated to the program.
* Recognise if it is audio or video stream reading the PES packet field ```stream_id```


# Performance

| Step	| Real time  | User time | Sys |
| Initial time | 2.13 | 1.45 | 0.65 |
| Packetize the read with 188 sized blocks  | 1.63 | 1.10 | 0.35 |
| Postpone the write  | 0.21 | 0.15 | 0.06 |



 

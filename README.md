nodeimgseek
===========

imgSeek library port to Node.js

Manual
===========

 * How to compile?

> node-waf configure build

 * Where is settings?

See settings.js

 * How to run?

For testing purposes:
> node isk-daemon.js

As service (kinda)
> nohup node isk-daemon.js & > nohup.out

 * How to setup database?

Make these two requests:
a. Creates db space
curl http://127.0.0.1:31128/?op=createdb&dbid=1

b. Saves database
curl http://127.0.0.1:31128/?op=savealldbs

c. Now you can use database in gallery

API reference:
=============

 * Implemented methods at this state:

  * loadalldbs
    Not callable, service initializes it at start

  * createdb
  
    API:
    http://127.0.0.1:31128/?op=createdb&dbid=1
    Returns:
    true - success
    false - failed (perhaps db space exists)

 * savealldbs
  API:
  http://127.0.0.1:31128/?op=savealldbs

 * Need to finish describe these, currently all can be viewed at isk-daemon.js
   * resetdb
   * removedb
   * getimgcount
   * addimage
   * isimageondb
   * queryimgid
   * removeid
   * isvaliddb
   * queryimgfile
   * getdblist


Live example:
=============
 * http://artmight.com/similar/image/264052
 * http://dressworlds.com/similar/image/8706

Implementation examples:
 * http://code.google.com/p/hppg/


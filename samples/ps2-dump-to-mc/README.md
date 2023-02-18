# ps2-dump-to-mc

## Extracting dumped contents

You can export the dumped file after executing the payload within Okage by using the following commands:

~~~
└─$ mymcplus -i Mcd001.ps2 export BASCUS-97129
Exporing BASCUS-97129 to BASCUS-97129.psu

└─$ psu export BASCUS-97129.psu dump.bin
[+] dump.bin exported to dump.bin

└─$ cat dump.bin      
LoveFlag00
~~~

The [mymcplus](https://pypi.org/project/mymcplus/) and [psu](https://github.com/McCaulay/pypsu) commands are Python applications installed by pip as they are dependencies of the Okager application.
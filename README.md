# StoreSearch
An OS course project to filtering files content and sorting them ascend/descend by field.

## How it works ?
First process is a `LoadBalancer` which balance inputs file into workers processes. Then `Worker` processes filter files content by inputs and send them to a `Presenter` process. Presenter sort contents and merge them to last inputs. All interconnections and message passing are using by pipe. (Unnamed pipes between loadBalancer and workers , Named pipe between workers and presenter).

## How to run ?
In project repository run `make` command to make project by g++.

## Inputs 
After running `Search` executable file your inputs should be like this : 

```
(<field name> = <corresponding filtering value> - )*( <sorting value = ascend/descend> - )? ￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼prc_cnt = n – dir = <relative dataset address> 
```

And input files in input directory should be `*.dms` with contents like this : 

```
brand model ram hdd price 
apple pro 16 512 30
apple pro 16 246 26
```

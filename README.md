# Game Search
An OS course project for filtering files contents and sorting them ascending/descending by specified field.

## How it works ?
First process is a `load_balancer` which balance sends filenames and filter fields to `worker` processes and also sends sort field and sort direction to a `presenter` process. Then `worker` processes filter files content by inputs and send them to the `presenter` process. Presenter sorts contents and merges them to last inputs. All interconnections and message passing are using by pipe. (Unnamed pipes between load_balancer and workers , Named pipe between workers and presenter and also between load_balancer and presenter).

## How to run ?
In project repository run `make` command to make project by g++.

## Commands
After running `load_balancer` executable file, your commands should be like this : 

```
(<field-name> = <query-value> - )*(<field-to-sort-by> = ascending|descending - )?￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼processes = <number-of-processes> – dir = <relative-dataset-address> 
```

And input files in input directory should be `dataset*` with contents like this : 

```
Name - Platform - Year - Genre - Publisher - NA_Sales - EU_Sales - JP_Sales - Other_Sales - Global_Sales
Wii Sports - Wii - 2006 - Sports - Nintendo - 41.49 - 29.02 - 3.77 - 8.46 - 82.74
Final Fantasy X - PS2 - 2001 - Role_Playing - Sony Computer Entertainment - 2.91 - 2.07 - 2.73 - 0.33 - 8.05
Minecraft - PS3 - 2014 - Misc - Sony Computer Entertainment - 1.97 - 2.51 - 0 - 0.94 - 5.42
```

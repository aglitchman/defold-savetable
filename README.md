# SaveTable

This asset allows you to serialize/deserialize Lua tables in Defold the same way as it does via `sys.save`/`sys.load`. The limits are the same:

> Internally, this function uses a workspace buffer sized output file sized 512kb. This size reflects the output file size which must not exceed this limit. Additionally, the total number of rows that any one table may contain is limited to 65536 (i.e. a 16 bit range). When tables are used to represent arrays, the values of keys are permitted to fall within a 32 bit range, supporting sparse arrays, however the limit on the total number of rows remains in effect.

## Installation

Use it in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/). Open your `game.project` file and in the dependencies field under project add:

https://github.com/aglitchman/defold-savetable/archive/main.zip

## Usage

```lua
local tbl = { example = true }

-- Encode Lua table into string
local str = savetable.stringify(tbl)
print(str)

-- Decode Lua table from string
local tbl2 = savetable.parse_string(str)
pprint(tbl2)
```

## Credits

This project is licensed under the terms of the CC0 1.0 Universal license.

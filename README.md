# SaveTable

⚠️ Defold has implemented [`sys.serialize()`/`sys.deserialize()`](https://defold.com/ref/beta/sys/#sys.serialize:table) since version 1.2.188. Use the new functions instead of this extension. ⚠️

## Description

This asset allows you to serialize/deserialize Lua tables in Defold the same way as it does via `sys.save`/`sys.load`. The limits are the same:

> Internally, this function uses a workspace buffer sized output file sized 512kb. This size reflects the output file size which must not exceed this limit. Additionally, the total number of rows that any one table may contain is limited to 65536 (i.e. a 16 bit range). When tables are used to represent arrays, the values of keys are permitted to fall within a 32 bit range, supporting sparse arrays, however the limit on the total number of rows remains in effect.

## Installation

Use it in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/). Open your `game.project` file and in the dependencies field under project add:

https://github.com/aglitchman/defold-savetable/archive/main.zip

## Usage

```lua
local tbl = { example = true }

-- Encode Lua table into a base64-encoded string
local str = savetable.encode_base64(savetable.serialize(tbl))
print(str)

-- Decode Lua table from a string
local tbl2 = savetable.deserialize(savetable.decode_base64(str))
pprint(tbl2)
```

### How to use it in [DefSave](https://github.com/subsoap/defsave) for HTML5 builds

`defsave/defsave.lua`, replace lines 124-131 with:

```lua
	local loaded_file
	if html5 then
		-- sys.load can't be used for HTML5 apps running on iframe from a different origin (cross-origin iframe)
		-- use `localStorage` instead because of this limitation on default IndexedDB storage used by Defold
		local EMPTY_TABLE = savetable.encode_base64(savetable.serialize({}))
		loaded_file = savetable.deserialize(savetable.decode_base64(html5.run([[(function(){try{return window.localStorage.getItem(']] .. path .. [[')||']] .. EMPTY_TABLE .. [['}catch(e){return']] .. EMPTY_TABLE .. [['}})()]])))
	else
		loaded_file = sys.load(path)
	end
```

`defsave/defsave.lua`, replace lines 183-194 with:

```lua
	local is_save_successful
	if html5 then
		-- sys.save can't be used for HTML5 apps running on iframe from a different origin (cross-origin iframe)
		-- use `localStorage` instead because of this limitation on default IndexedDB storage used by Defold
		local encoded_data = savetable.encode_base64(savetable.serialize(M.loaded[file].data))
		html5.run([[try{window.localStorage.setItem(']] .. path .. [[', ']] .. encoded_data .. [[')}catch(e){}]])

		is_save_successful = true
	else
		is_save_successful = sys.save(path, M.loaded[file].data)
	end
```

## Credits

This project is licensed under the terms of the CC0 1.0 Universal license.

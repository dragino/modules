--[[

    xively.lua - Lua Script to set messageto xively 

    Copyright (C) 2013 edwin chen <edwin@dragino.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

]]--

--Replace FEED and API_KEY according to your xively info
FEED = "xxxxxxx"
API_Key = "xxxxxxxxx"


msg = arg[1]
msg = '\"' .. msg.. '\"'
API_String = "\"X-ApiKey: " .. API_Key .. "\" "
URL = "https://api.xively.com/v2/feeds/"
FeedURL = URL..FEED..".csv"

CMD_UPDATE_FEED =  'curl -k --request PUT --data-binary ' .. msg .. ' --header ' ..  API_String .. FeedURL
os.execute("echo ".. CMD_UPDATE_FEED ..">> /tmp/logtest")
os.execute(CMD_UPDATE_FEED)			--update feeds
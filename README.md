# Memory Surf - Memory Reader and realtime statistics tracker for Audiosurf (2008)
---
>[!IMPORTANT]
>For now this tool stays into a deep development state and there is not actual working stable build
>
> If you want to use something of it, please wait untill some kind of stable release (it will be released, *someday*)
>
> If you have an ideas how to improve or rework some parts of this tool, feel free to fork, open pull requests\issues, etc.
---
- Based on [Quest3DTamperer](https://github.com/AudiosurfResearch/Quest3DTamperer) ideas, **MemorySurf** is a specialized tool for extracting player statistics from game in real time. It also provides a web socket server on localhost to broadcasting this data to any listener who wants to use this data (OBS Plugins, other audiosurf assistant tools, whoever)
---
## Features
- Extracting game statistics:
  - Total amount and collected counts of colored blocks traffic (actually mostly for Elite puzzles)
  - Time elapsed sinse song start (ignores pauses)
  - Total player score
  - Max match
  - (not quite) current and maximum chain strength
- Web socket server that broadcasts readed data on localhost:port_that_you_selected (by default use localhost:20080)

## Planned features
- Extracting specialized mono statistics
- Ready-to-use trackes for any Puzzle (Casual, Pro and Elite) and Mono modes
- **PUBLIC RELEASE**
---
## Plans for far future (may be even never implemented)
- Turn into a generic mod loader
- LuaJIT API for third-party plugins providing documented and kinda-easy-to-use interfaces to interact with game internals instead of using braindead Quest3D SDK and complicated reverse-engineering tools
- Initial project target (realtime statistics tracker) made as first Lua mod

## Credits
- **M1nt_** aka **RubberDuckShobe** aka **the_crescent_moon**, ([this dude](https://github.com/RubberDuckShobe)) for creating a [Quest3DTamperer](https://github.com/AudiosurfResearch/Quest3DTamperer) which was an inspiration and also developing tool for this project

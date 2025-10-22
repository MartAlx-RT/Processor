#pragma once



#define _print_err(Err) \
printf(colorize("Error\t", _RED_) colorize(Err, _YELLOW_))

#define _print_location(File, Line) \
printf(colorize("%s:%u\n",_GRAY_),File, Line+1)


#define _print_wrg(Wrg) \
printf(colorize("Warning\t", _CYAN_) colorize(Wrg, _GRAY_))

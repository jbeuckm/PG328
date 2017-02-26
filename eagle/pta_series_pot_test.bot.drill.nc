(.../pcb-gcode-3.6.2.4/pcb-gcode.ulp)
(Copyright 2005 - 2012 by John Johnson)
(See readme.txt for licensing terms.)
(This file generated from the board:)
(.../atmega_midi_pg800/eagle/pta_series_pot_test.brd)
(Current profile is .../pcb-gcode-3.6.2.4/profiles/mach.pp  )
(This file generated 2/20/17 3:49 PM)
(Settings from pcb-machine.h)
(spindle on time = 0.0000)
(spindle speed = 20000.0000)
(tool change at 0.0000  0.0000  1.5000  )
(feed rate xy = F0     )
(feed rate z  = F20    )
(Z Axis Settings)
(  High     Up        Down     Drill)
(0.2500  	0.0500  	-0.0035 	-0.0710 )
(Settings from pcb-defaults.h)
(isolate min = 0.0040)
(isolate max = 0.0200)
(isolate step = 0.0025)
(Generated bottom outlines, bottom drill, )
(Unit of measure: inch)
( Tool|       Size       |  Min Sub |  Max Sub |   Count )
( T01 | 1.000mm 0.0394in | 0.0000in | 0.0000in |       0 )
( T02 | 1.200mm 0.0472in | 0.0000in | 0.0000in |       0 )
( T03 | 1.700mm 0.0669in | 0.0000in | 0.0000in |       0 )
(Inch Mode)
G20
(Absolute Coordinates)
G90
S20000
G00 Z0.2500  
G00 X0.0000  Y0.0000  
M03
G04 P0.000000
M05
G00 Z1.5000  
M06 T01  ; 0.0394  
G01 Z0.0000  F20    
M06 
G00 Z0.0500  
M03
G04 P0.000000
G82 X-0.1933 Y0.2500  Z-0.0710 F20    R0.0500  P0.250000
G82 X-0.3311 Y0.2500  
G82 X-0.4689 Y0.2500  
G82 X-0.6067 Y0.2500  
M05
G00 Z1.5000  
G00 X0.0000  Y0.0000  
M06 T02  ; 0.0472  
G01 Z0.0000  F20    
M06 
G00 Z0.0500  
M03
G04 P0.000000
G82 X-0.2811 Y0.4937  Z-0.0710 F20    R0.0500  P0.250000
G82 X-0.4189 Y0.4937  
G82 X-0.2811 Y2.8063  
M05
G00 Z1.5000  
G00 X0.0000  Y0.0000  
M06 T03  ; 0.0669  
G01 Z0.0000  F20    
M06 
G00 Z0.0500  
M03
G04 P0.000000
G82 X-0.1846 Y0.7043  Z-0.0710 F20    R0.0500  P0.250000
G82 X-0.5154 Y0.6571  
G82 X-0.1846 Y2.5957  
G82 X-0.5154 Y2.6429  
T01 
G00 Z0.2500  
M05
M02

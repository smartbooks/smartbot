onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /flow_led_tb/u0_flow_led/sys_clk
add wave -noupdate /flow_led_tb/u0_flow_led/sys_rst_n
add wave -noupdate -expand /flow_led_tb/u0_flow_led/led
add wave -noupdate /flow_led_tb/u0_flow_led/counter
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {9187 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 244
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {999932 ns} {1000004 ns}

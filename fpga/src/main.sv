`timescale 1ns/1ns

module main(
			input logic 			enable,
			input logic [ 6 : 0 ] 	command,
			input logic [ 7 : 0 ] 	data,

			output logic [ 34 : 0 ] rleds,
			output logic [ 34 : 0 ] gleds,
			output logic [ 17 : 0 ] adcs
			);  

  localparam FPGA_ADC_SELECT_CMD = 7'h1;
  localparam FPGA_RED_LED_ON = 7'h2;   
  localparam FPGA_RED_LED_OFF = 7'h3;
  localparam FPGA_GREEN_LED_ON = 7'h4;   
  localparam FPGA_GREEN_LED_OFF = 7'h5;

  initial begin
	rleds = 35'h000000000;
	gleds = 35'h000000000;
	adcs = 18'h00000;
  end
  
  function [ 17 : 0 ] fpga_adc_select_func;	
    input [ 7 : 0 ] 	data_arg;
	input [ 6 : 0 ] 	command_arg;
	input [ 17 : 0 ] 	adcs_arg;

	if( command_arg == FPGA_ADC_SELECT_CMD ) fpga_adc_select_func = ( 35'h000000001 << ( data_arg / 2 ));
	else fpga_adc_select_func = adcs_arg;
    
  endfunction // fpga_adc_select_func
  
  function [ 34 : 0 ] fpga_green_led_on_off_func;
	input [ 7 : 0 ] 	data_arg;
	input [ 6 : 0 ] 	command_arg;
	input [ 34 : 0 ] 	gleds_arg;
    
	if( command_arg == FPGA_GREEN_LED_ON ) fpga_green_led_on_off_func = ( gleds_arg | ( 35'h000000001 << data_arg ));
	else if( command_arg == FPGA_GREEN_LED_OFF ) fpga_green_led_on_off_func = ( gleds_arg & ( ~( 35'h000000001 << data_arg )));
	else fpga_green_led_on_off_func = gleds_arg;
	
  endfunction // fpga_green_led_on_off_func
  
  function [ 34 : 0 ] fpga_red_led_on_off_func;
	input [ 7 : 0 ] 	data_arg;
	input [ 6 : 0 ] 	command_arg;
	input [ 34 : 0 ] 	rleds_arg;

	if( command_arg == FPGA_RED_LED_ON ) fpga_red_led_on_off_func = ( rleds_arg | ( 35'h000000001 << data_arg ));
	else if( command_arg == FPGA_RED_LED_OFF ) fpga_red_led_on_off_func = ( rleds_arg & ( ~( 35'h000000001 << data_arg )));
	else fpga_red_led_on_off_func = rleds_arg;
	
  endfunction // fpga_red_led_on_off_func
  
  always @ ( posedge enable ) begin
    
	adcs = fpga_adc_select_func( data, command, adcs );
	gleds = fpga_green_led_on_off_func( data, command, gleds );
	rleds = fpga_red_led_on_off_func( data, command, rleds );
	
  end
  
endmodule // main

`timescale 1ns/1ns
`include "main.sv"

module tb_main(		
					output logic [ 6 : 0 ] command,
					output logic [ 7 : 0 ] data,
					output logic 		   enable,

					input logic [ 34 : 0 ] rleds,
					input logic [ 34 : 0 ] gleds,
					input logic [ 17 : 0 ] adcs
					);  
  
  localparam FPGA_ADC_SELECT_CMD = 7'h1;
  localparam FPGA_RED_LED_ON = 7'h2;   
  localparam FPGA_RED_LED_OFF = 7'h3;
  localparam FPGA_GREEN_LED_ON = 7'h4;   
  localparam FPGA_GREEN_LED_OFF = 7'h5;
  localparam CHANNELS_COUNT = 8'd35;

  integer 								   i = 8'h00;
  
  main main_inst( .command( command ), .data( data ), .enable( enable ), .rleds( rleds ), .gleds( gleds ), .adcs( adcs ));

  initial begin
	command = 7'h00;
	data = 8'h00;
	enable = 1'b0;	
  end

  always begin

	#1 command = FPGA_ADC_SELECT_CMD;
	for( i = 0; i < CHANNELS_COUNT; i = i + 1 ) begin
	  
	  #1 data = i; 
	  $display( "Select ADC[%d] command.", data );
	  #1 enable = 1'b1;
	  #1 enable = 1'b0;
	  #5;
	  
	end

	#1 command = FPGA_RED_LED_ON;
	for( i = 0; i < CHANNELS_COUNT; i = i + 1 ) begin
	  
	  #1 data = i; 
	  $display( "RLED[%d] ON command.", data );
	  #1 enable = 1'b1;
	  #1 enable = 1'b0;
	  #5;
	  
	end

	#1 command = FPGA_RED_LED_OFF;
	for( i = 0; i < CHANNELS_COUNT; i = i + 1 ) begin
	  
	  #1 data = i; 
	  $display( "RLED[%d] OFF command", data );
	  #1 enable = 1'b1;
	  #1 enable = 1'b0;
	  #5;
	  
	end

	#1 command = FPGA_GREEN_LED_ON;
	for( i = 0; i < CHANNELS_COUNT; i = i + 1 ) begin
	  
	  #1 data = i; 
	  $display( "GLED[%d] ON command.", data );
	  #1 enable = 1'b1;
	  #1 enable = 1'b0;
	  #5;
	  
	end

	#1 command = FPGA_GREEN_LED_OFF;
	for( i = 0; i < CHANNELS_COUNT; i = i + 1 ) begin
	  
	  #1 data = i; 
	  $display( "GLED[%d] OFF command.", data );
	  #1 enable = 1'b1;
	  #1 enable = 1'b0;
	  #5;
	  
	end	  	  	  
  end

  initial begin
	
	#5000 $finish;
	
  end  
  
  initial begin
	
	$dumpfile( "tb_main.vcd" );
	$dumpvars( 0, tb_main );
	$monitor( $stime,, command,, data,, enable,, rleds,, gleds,, adcs );
	
  end
  
endmodule

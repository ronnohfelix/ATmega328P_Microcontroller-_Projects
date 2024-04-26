; Espresso Machine.asm
.include "m328pdef.inc" // to include atmega328p

.equ COFFEE_TEMP_SETPOINT = 234 ; Corresponds to 93.4 degrees Celsius
.equ COFFEE_TEMP_LOWER_LIMIT = 213 ; Corresponds to 85 degrees Celsius
.equ STEAM_TEMP_LIMIT = 368 ; Corresponds to 184 degrees Celsius
.equ INFUSION_TEMP_LIMIT = 341 ; Corresponds to 135 degrees Celsius
.equ PUMP_PRESSURE_THRESHOLD = 51 ; Corresponds to 0.5 bar
.equ STEAM_PRESSURE_THRESHOLD = 92 ; Corresponds to 0.9 bar

ldi r16, 0xFF ; Initialize port B for output
out DDRB, r16

ldi r16, 0x00 ; Initialize port C for input
out DDRC, r16

ldi r16, 0x00 ; Initialize port D for input
out DDRD, r16
ldi r16, (1<<PD2)|(1<<PD3)|(1<<PD4) ; Enabling external interrupts for PD2, PD3, PD4
out EIMSK, r16
ldi r16, (1<<ISC21)|(1<<ISC31)|(1<<ISC41) ; Setting interrupts to trigger on falling edge for PD2, PD3, PD4
out EICRA, r16

ldi r16, (1<<ADEN)|(1<<ADLAR)|(1<<REFS0)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
out ADCSRA, r16 ; Enabling ADC, left adjust result, AVCC reference, enable ADC interrupt, set prescaler to 128

sei ; Enabling global interrupts

start:
  rcall read_sensors ; Reading temperatures and pressures from sensors
  rcall control_coffee_boiler ; Controlling coffee boiler temperature
  rcall control_steam_boiler ; Controlling steam boiler temperature
  rcall control_infusion_group ; Controlling infusion group temperature
  rcall control_pump_pressure_led ; Controlling pump water pressure LED
  rcall control_steam_pressure_led ; Controlling boiler steam pressure LED
  rjmp start

read_sensors:
  ldi r16, (1<<MUX0)|(1<<REFS0) ; Select PC0 as ADC input, set AVCC reference
  out ADMUX, r16
  ldi r16, (1<<ADSC)|(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) ; Start ADC conversion, enable ADC, enable ADC interrupt, set prescaler to 128
  out ADCSRA, r16
  ret

control_coffee_boiler:
  cpi r22, COFFEE_TEMP_SETPOINT ; Comparing temperature with setpoint
  brge coffee_temp_ok ; If greater or equal, skip turning on LED
  sbi PORTB, 2 ; Else turn on Coffee temperature LED
  rjmp end_control_coffee_boiler
coffee_temp_ok:
  cpi r22, COFFEE_TEMP_LOWER_LIMIT ; Comparing temperature with lower limit
  brge end_control_coffee_boiler ; If greater or equal, skip turning off heating element
  cbi PORTB, 4 ; Else turn off Coffee boiler heating element
end_control_coffee_boiler:
  ret

control_steam_boiler:
  cpi r23, STEAM_TEMP_LIMIT ; Comparing temperature with limit
  brge turn_off_steam_boiler ; If greater or equal, turn off steam boiler
  rjmp end_control_steam_boiler
turn_off_steam_boiler:
  cbi PORTB, 5 ; Turning off Steam boiler heating element
end_control_steam_boiler:
  ret

control_infusion_group:
  cpi r24, INFUSION_TEMP_LIMIT ; Comparing temperature with limit
  brge turn_off_infusion_group ; If greater or equal, turn off infusion group
  rjmp end_control_infusion_group
turn_off_infusion_group:
  cbi PORTB, 6 ; Turning off Infusion group heating element
end_control_infusion_group:
  ret

control_pump_pressure_led:
  cpi r25, PUMP_PRESSURE_THRESHOLD ; Comparing pressure with threshold
  brge pump_pressure_ok ; If greater or equal, skip turning on LED
  sbi PORTB, 0 ; Turn on Pump pressure LED
  ldi r26, 15 ; Load 15 into r26 for 15 seconds delay
  call delay_1s_loop ; Call delay subroutine
  cbi PORTB, 0 ; Turn off Pump pressure LED after delay
pump_pressure_ok:
  ret

control_steam_pressure_led:
  cpi r27, STEAM_PRESSURE_THRESHOLD ; Comparig pressure with threshold
  brge steam_pressure_ok ; If greater or equal, skip turning on LED
  sbi PORTB, 1 ; Turn on Steam pressure LED
  rjmp end_control_steam_pressure_led
steam_pressure_ok:
  cbi PORTB, 1 ; Turn off Steam pressure LED
end_control_steam_pressure_led:
  ret

ADC_vect:
  in r22, ADCH ; Read coffee boiler temperature
  in r23, ADCH ; Read steam boiler temperature
  in r24, ADCH ; Read infusion group temperature
  in r25, ADCH ; Read pump water pressure
  in r26, ADCH ; Read boiler steam pressure
  reti

delay_1s_loop:
  ldi r20, 30 ; Load 30 into r20 for outer loop (about 1s delay)
  outer_loop:
    ldi r21, 250 ; Load 250 into r21 for inner loop (about 1ms delay)
    inner_loop:
      dec r21 ; Decrement inner loop counter
      brne inner_loop ; Continue inner loop if counter not zero
    dec r20 ; Decrement outer loop counter
    brne outer_loop ; Continue outer loop if counter not zero
  ret

ISR(INT0_vect) ; Emergency button interrupt
  cbi PORTB, 4 ; Turn off Coffee boiler heating element
  cbi PORTB, 5 ; Turn off Steam boiler heating element
  cbi PORTB, 6 ; Turn off Infusion group heating element

ISR(INT1_vect) ; Heating elements push button interrupt
  sbi PORTB, 4 ; Turn on Coffee boiler heating element
  sbi PORTB, 5 ; Turn on Steam boiler heating element
  sbi PORTB, 6 ; Turn on Infusion group heating element

ISR(INT2_vect) ; Standby mode button interrupt
  ldi r16, 0x00 ; Reset counter
  out TCNT0, r16
  ldi r16, (1<<CS02)|(1<<CS00) ; Set prescaler to 1024
  out TCCR0B, r16
  ldi r16, (1<<TOIE0) ; Enable overflow interrupt
  out TIMSK0, r16
  reti

ISR(TIMER0_OVF_vect) ; Timer overflow interrupt
  inc r16 ; Increment counter
  cpi r16, 195 ; Compare with 5 seconds
  breq awaken_machine ; If equal, awaken machine
  reti

awaken_machine:
  sbi PORTB, 3 ; Turn on AE Alarm LED
  reti

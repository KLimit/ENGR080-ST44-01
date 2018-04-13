#include <PControl.h>
#include <Logger.h>
#include <EnvADC.h>

Logger logger;
EnvADC env;

void setup() {
  // put your setup code here, to run once:

  logger.include(&env);
}

void loop() {
  // put your main code here, to run repeatedly:
  

}

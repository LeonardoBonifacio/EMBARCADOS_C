Este código implementa a logica de um sinal de transito em que os leds são acesos com 3 segundos de diferença entre vermelho,amarelo e verde, utilizando um temporizador repetitivo que apaga todos os leds de 3 em 3 segundos e baseado na variavel 'estado' o led correto é acesso. A cada segundo é printado no terminal "sinaleira funcionando", se o led vermelho está acesso é printado "Sinal fechado", se o led amarelo está acesso é printado "Sinal abrindo ou vai freiando" e se o led verde está acesso é printado "sinal aberto"




Para rodar este código é necessário ter o arquivo .c e o arquivo do cmakelist em uma pasta para que logo apos você utilize a extensão da raspberry pico para importar este projeto e poder compilá - lo e rodar tanto na sua BitDogLab(mudando os pinos do leds para vermelho == 13, amarelo == 13 e 11(liga os dois leds) e verde == 11)") ou através da extensão wokwi pelo vs code, tendo os arquivos diagram.json  e o wokwi.toml preenchidos.
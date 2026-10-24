# LiFi — projeto de PCOM

Comunicação óptica didática entre Arduino emissor e receptor. Inclui Lifi_Emissor, Lifi_Receptor e variante Lifi_Receptor_LCD com LiquidCrystal_I2C, LCD 16x2 no endereço 0x27.

O emissor transmite caracteres digitados na serial a 9600 baud por pulsos no pino 3; o receptor usa pino 2, com lógica invertida pelo circuito óptico. O protocolo implementa start, oito bits LSB primeiro e intervalo de stop, com BIT_DELAY de 50 ms. A serial de 9600 não é a taxa do enlace óptico. Igualar os tempos das duas placas e conferir circuito antes de usar. Não foram executados testes em hardware nesta curadoria.

Relatório localizado no Drive: https://docs.google.com/document/d/16ybOgAsTEMch38dSzpslWZ-wYW7XMRwSygL5OV8x6pI/edit
Apresentação: https://docs.google.com/presentation/d/18Tn16DfMYXIA8WSiOokkBng4S6_sy-ceQytOpuX0W0Q/edit

Confirmar créditos da equipe, participação individual, fotos e resultados medidos antes de publicar.

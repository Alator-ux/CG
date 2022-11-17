with open("./images/texture.jpg", "rb") as image:
  f = image.read()
  b = bytearray(f)
  
with open("./images/decoded.txt", "wb") as output:
    output.write(b)

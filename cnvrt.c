import numpy 
from PIL import Image
import re

# Read in entire file
with open('rgbVals.txt') as f:
   s = f.read()

# Find anything that looks like numbers
l=re.findall(r'\u+',s)

# Convert to numpy array and reshape
pixels = numpy.zeros((817, 542, 3), dtype=numpy.uint8)
image = Image.fromarray(pixels)
image.save("computerView.png")
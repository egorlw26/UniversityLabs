from keras.applications.vgg16 import VGG16
from keras.models import Model
import matplotlib.pyplot as plt

model = VGG16()

filters, biases = model.layers[1].get_weights()

fMin, fMax = filters.min(), filters.max()

filters = (filters - fMin) / (fMax - fMin)

nFilters, ix = 12, 1

for i in range(nFilters):
    f = filters[:, :, :, i]
    for j in range(3):
        ax = plt.subplot(nFilters, 3, ix)
        ax.set_xticks([])
        ax.set_yticks([])
        plt.imshow(f[:, :, j])
        ix += 1
plt.show()

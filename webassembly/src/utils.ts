const Module = globalThis.Module;

export interface AppCoordinate {
  x: number;
  y: number;
  width: number;
  height: number;
}

const passToWasm = (uint8ArrData: Uint8Array) => {
  const numBytes = uint8ArrData.length * uint8ArrData.BYTES_PER_ELEMENT;
  const dataPtr = Module._malloc(numBytes);
  const dataOnHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, numBytes);
  dataOnHeap.set(uint8ArrData);
  return {
    byteOffset: dataOnHeap.byteOffset,
    length: uint8ArrData.length,
  };
};

const loadImage = async (imgBlob: Blob) => {
  const img = document.createElement("img");
  const promise = new Promise<ImageData>((resolve) => {
    img.onload = () => {
      const canvas = document.createElement("canvas");
      canvas.width = img.width;
      canvas.height = img.height;

      const ctx = canvas.getContext("2d");
      ctx?.drawImage(img, 0, 0);
      const imageData = ctx?.getImageData(0, 0, canvas.width, canvas.height);
      imageData && resolve(imageData);
    };
  });
  img.src = URL.createObjectURL(imgBlob);
  return promise;
};

export const processImages = async (imgBlob: Blob, templBlob: Blob) => {
  const img = await loadImage(imgBlob);
  const templ = await loadImage(templBlob);
  const imgUint = passToWasm(new Uint8Array(img.data.buffer));
  const templUint = passToWasm(new Uint8Array(templ.data.buffer));

  try {
    const resultPtr = Module._check_image(
      imgUint.byteOffset,
      img.width,
      img.height,
      templUint.byteOffset,
      templ.width,
      templ.height
    );

    const result = new Int32Array(Module.asm.memory.buffer, resultPtr, 4);
    return {
      x: result[0] - templ.width,
      width: result[1],
      y: result[2] - templ.height,
      height: result[3],
    };
  } catch (e) {
    console.log({ e });
  }
};

import matlab.engine

eng = matlab.engine.start_matlab()

img_enc = eng.func_SPIHT_Main()

print len(img_enc[0])

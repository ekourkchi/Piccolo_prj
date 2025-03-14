#################################
INT16_MIN = -32768
INT16_MAX = 32767

def float_to_int16(accel: float, accel_range=(-1, 2)) -> int:
   
    ret = INT16_MIN + (INT16_MAX - INT16_MIN) * (accel - accel_range[0]) / (accel_range[1] - accel_range[0])

    ret = int(ret)   # round it or cast it as int16

    return ret
#################################

print(float_to_int16(0.39))
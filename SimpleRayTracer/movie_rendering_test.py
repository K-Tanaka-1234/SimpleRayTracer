import ctypes
import numpy as np
import cv2
from tqdm import tqdm

class vec3(ctypes.Structure):
    _fields_ = [("e", ctypes.c_double*3)]

class renderer_config(ctypes.Structure):
    _fields_ = [("rendering_width", ctypes.c_int),
                ("rendering_height", ctypes.c_int),
                ("num_channels", ctypes.c_int),
                ("num_samples", ctypes.c_int),
                ("ray_max_depth", ctypes.c_int),
                ("background", vec3)]

class camera_config(ctypes.Structure):
    _fields_ = [("vfov", ctypes.c_double),
                ("lookfrom", vec3),
                ("lookat", vec3),
                ("aperture", ctypes.c_double),
                ("focus_dist", ctypes.c_double)]

class texture(ctypes.Structure):
    _fields_ = [("tex_value", ctypes.c_void_p),
                ("color", vec3),
                ("bmp_ptr", ctypes.POINTER(ctypes.c_ubyte)),
                ("bmp_cols", ctypes.c_int),
                ("bmp_rows", ctypes.c_int)]

class material(ctypes.Structure):
    _fields_ = [("scatter", ctypes.c_void_p),
                ("emitted", ctypes.c_void_p), 
                ("tex", texture),
                ("fuzziness", ctypes.c_double),
                ("ri", ctypes.c_double)]

class object_data(ctypes.Structure):
    _fields_ = [("type", ctypes.c_int),
                ("data", ctypes.c_double*4),
                ("center", vec3),
                ("mat", ctypes.POINTER(material))]

class object(ctypes.Structure):
    _fields_ = [("obj_data", object_data),
                ("intersection_test", ctypes.c_void_p),
                ("get_bounding_box", ctypes.c_void_p)]

def to_c_array_d(list):
    return (ctypes.c_double*len(list))(*list)

mp4_out = "result.mp4"
window_name = "test_window"
rows = 720
cols = 1280
#rows = 1080
#cols = 1920

img = cv2.cvtColor(cv2.imread("PavingStones070_2K_Color.jpg"), cv2.COLOR_BGR2RGB)
#img = cv2.cvtColor(cv2.imread("_2k_earth_daymap.jpg"), cv2.COLOR_BGR2RGB)
#img = cv2.cvtColor(cv2.imread("test_tex.png"), cv2.COLOR_BGR2RGB)
img_f = img.flatten().copy()

test_dll = ctypes.cdll.LoadLibrary("../x64/Release/SimpleRayTracer.dll")
test_dll.init(rows, cols)
buffer = np.zeros((rows * cols * 3), dtype=np.uint8)
test_dll.enable_direct_frame_buffer(buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)))

#
#   RENDERER CONFIG ---------------------------------------------------------------------------------------------------------------------------------------------------------
#

r_conf = renderer_config(rendering_width=cols, 
                            rendering_height=rows, 
                            num_channels=3, 
                            num_samples=3, 
                            ray_max_depth=10,
                            background=vec3(to_c_array_d([0.0, 0.0, 0.0])))

#
#   OBJECTS -----------------------------------------------------------------------------------------------------------------------------------------------------------------
#
t0 = texture()
m0 = material()
obj0 = object()

test_dll.create_solid_color_texture(vec3(to_c_array_d([0.6, 0.8, 0.8])), ctypes.byref(t0))
test_dll.create_metal_material(t0, ctypes.c_double(0.05), ctypes.byref(m0))
test_dll.create_sphere_object(vec3(to_c_array_d([-1.1, 0.1, -1.1])), ctypes.c_double(0.15), ctypes.byref(obj0))
test_dll.add_object(obj0, m0)

t0_1 = texture()
m0_1 = material()
obj0_1 = object()

test_dll.create_solid_color_texture(vec3(to_c_array_d([0.8, 0.6, 0.6])), ctypes.byref(t0_1))
test_dll.create_lambertian_material(t0_1, ctypes.byref(m0_1))
test_dll.create_sphere_object(vec3(to_c_array_d([-0.9, -0.1, -0.9])), ctypes.c_double(0.15), ctypes.byref(obj0_1))
test_dll.add_object(obj0_1, m0_1)

t1 = texture()
m1 = material()
obj1 = object()

test_dll.create_bitmap_texture(img_f.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)), ctypes.c_int(img.shape[1]), ctypes.c_int(img.shape[0]), ctypes.byref(t1))
test_dll.create_lambertian_material(t1, ctypes.byref(m1))
test_dll.create_sphere_object(vec3(to_c_array_d([0.2, -0.5, -0.9])), ctypes.c_double(0.9), ctypes.byref(obj1))
test_dll.add_object(obj1, m1)

t2 = texture()
m2 = material()
obj2 = object()
obj2_1 = object()

test_dll.create_solid_color_texture(vec3(to_c_array_d([0.93, 0.93, 0.93])), ctypes.byref(t2))
test_dll.create_dielectric_material(t2, ctypes.c_double(1.5), ctypes.byref(m2))
test_dll.create_sphere_object(vec3(to_c_array_d([-1.0, 0.0, -1.0])), ctypes.c_double(0.4), ctypes.byref(obj2))
test_dll.create_sphere_object(vec3(to_c_array_d([-1.0, 0.0, -1.0])), ctypes.c_double(-0.35), ctypes.byref(obj2_1))
test_dll.add_object(obj2, m2)
test_dll.add_object(obj2_1, m2)

t3 = texture()
m3 = material()
obj3 = object()

test_dll.create_solid_color_texture(vec3(to_c_array_d([1.0, 1.0, 1.0])), ctypes.byref(t3))
test_dll.create_diffuse_light_material(t3, ctypes.byref(m3))
test_dll.create_sphere_object(vec3(to_c_array_d([0.0, -21.5, -1.0])), ctypes.c_double(20.0), ctypes.byref(obj3))
test_dll.add_object(obj3, m3)

t4 = texture()
m4 = material()
obj4 = object()

test_dll.create_solid_color_texture(vec3(to_c_array_d([1.0, 0.7, 0.2])), ctypes.byref(t4))
test_dll.create_diffuse_light_material(t4, ctypes.byref(m4))
test_dll.create_sphere_object(vec3(to_c_array_d([-0.5, 1.5, -1.0])), ctypes.c_double(1.0), ctypes.byref(obj4))
test_dll.add_object(obj4, m4)

#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
t_len = 10
fps = 30
T = 1.0/fps
out = cv2.VideoWriter(mp4_out, cv2.VideoWriter_fourcc('m', 'p', '4', 'v'), fps, (cols, rows))

#frame = np.zeros((rows, cols, 3), dtype=np.float64)
cnt = 1

la = [-0.5, 0.0, -1.0]
t = 6.7

for i in tqdm(range(t_len*fps)):
    lf = [3.5*np.sin(t)+la[0], np.sin(2.0*t), 3.5*np.cos(t)+la[2]]
    c_conf = camera_config(vfov=20, 
                            lookfrom=vec3(to_c_array_d(lf)), 
                            lookat=vec3(to_c_array_d(la)),
                            aperture=0.2,
                            focus_dist=np.linalg.norm(np.array(la)-np.array(lf)))

    test_dll.clear_frame_buffer()
    test_dll.run_renderer(r_conf, c_conf)

    #frame += buffer.reshape(rows, cols, 3).astype(np.float64)
    #res = (frame/cnt).astype(np.uint8)

    res = buffer.reshape(rows, cols, 3)
    cv2.imshow(window_name, res)
    cv2.imwrite("tset.png", res)
    out.write(res)
    
    if cv2.waitKey(1) == 27:
        break

    cnt += 1
    t += T

out.release()
test_dll.free_frame_buffer()
test_dll.renderer_finish()
cv2.destroyAllWindows()
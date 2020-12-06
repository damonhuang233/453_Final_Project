class vertex:
    def __init__(self, x, y, z, s):
        self.x = x
        self.y = y
        self.z = z
        self.vx = 0.000000
        self.vy = 0.000000
        self.vz = 0.000000
        self.s = s

def format_vertex(x):
    return "{:.6f}".format(round(x, 6))

def _function(x, y, z, equation):
    return float(eval(equation))

def generate_data(equation, data, grid_len, num_p):

    data.append("ply")
    data.append("format ascii 1.0")

    num_ver = num_p * num_p * num_p
    line = "element vertex " + str(num_ver)
    data.append(line)

    data.append("property float64 x")
    data.append("property float64 y")
    data.append("property float64 z")
    data.append("property float64 vx")
    data.append("property float64 vy")
    data.append("property float64 vz")
    data.append("property float64 s")

    #num_edges = (num_p - 1) * num_p * num_p * 2 + num_p * num_p * (num_p - 1)
    num_face = (num_p - 1) * (num_p - 1) * num_p * 3
    line = "element face " + str(num_face)
    data.append(line)

    data.append("property list uint8 int32 vertex_indices")
    data.append("end_header")

    interval = grid_len / float(num_p - 1)
    tll = grid_len / 2. 
    tll_p = (-tll ,tll ,tll)

    vertexs = []
    for i in range(0, num_p):
        xy_plane = []
        for j in range(0, num_p):
            for k in range(0, num_p):
                v_x = tll_p[0] + float(k) * interval 
                v_y = tll_p[1] - float(j) * interval 
                v_z = tll_p[2] - float(i) * interval 
                v_s = _function(v_x, v_y, v_z, equation)
                new_vertex = vertex(v_x, v_y, v_z, v_s)
                xy_plane.append(new_vertex)
        vertexs.append(xy_plane)
    
    for xyp in vertexs:
        for p in xyp:
            line = format_vertex(p.x) + "\t"        \
                    + format_vertex(p.y) + "\t"     \
                    + format_vertex(p.z) + "\t"     \
                    + format_vertex(0.) + "\t"     \
                    + format_vertex(0.) + "\t"     \
                    + format_vertex(0.) + "\t"     \
                    + format_vertex(p.s)
            data.append(line)
    
    #clockwise rotation
    # faces on x_y plane
    for iz in range(0, num_p):
        for iy in range(0, num_p - 1):
            for ix in range(0, num_p - 1):
                n1 = iz * num_p * num_p + iy * num_p + ix
                n2 = n1 + 1
                n3 = n2 + num_p
                n4 = n3 - 1
                line = str(4) + " " + str(n1) + " " + str(n2) + " " + str(n3) + " " + str(n4)
                data.append(line)
    # faces on x_z plane
    for iy in range(0, num_p):
        for iz in range(0, num_p - 1):
            for ix in range(0, num_p - 1):
                n1 = iz * num_p * num_p + iy * num_p + ix
                n2 = n1 + 1
                n3 = n2 + num_p * num_p
                n4 = n3 - 1
                line = str(4) + " " + str(n1) + " " + str(n2) + " " + str(n3) + " " + str(n4)
                data.append(line)
    # faces on y_z plane
    for ix in range(0, num_p):
        for iz in range(0, num_p - 1):
            for iy in range(0, num_p - 1):
                n1 = iz * num_p * num_p + iy * num_p + ix
                n2 = n1 + num_p * num_p
                n3 = n2 + num_p
                n4 = n1 + num_p
                line = str(4) + " " + str(n1) + " " + str(n2) + " " + str(n3) + " " + str(n4)
                data.append(line)

def output_file(data, file_name):
    f = open(file_name, 'w')
    for line in data:
        line += "\n"
        f.write(line)
    f.close()
    print("output: ", file_name)
    

def setup():
    data = []
    
    file_name = input("Enter the output file name:")
    
    if len(file_name) < 5:
        file_name += ".ply"
    else:
        if file_name[-4:] != ".ply":
            file_name += ".ply"

    pass_check = False
    while not pass_check:
        print("Enter the scalar function: example: x * x + y * y + z * z")
        equation = input()
        try:
            res = _function(float(1), float(2), float(3), equation)
            print("x = 1., y = 2., z = 3., scalar = ", res)
            check = input("Y/N: \n")
            if check == "Y":
                pass_check = True
        except:
            print("Try again.") 

    while True:
        grid_len = input("Length of the 3D grid: ")
        grid_len = float(grid_len)
        if grid_len > 0:
            break
    
    while True:
        num_p = input("Number of vertexs on each axis: ")
        num_p = int(num_p)
        if num_p > 1:
            break

    generate_data(equation, data, grid_len, num_p)

    output_file(data, file_name)


setup()

import numpy as np 
background_color = np.array([255, 255, 255])
file_name = "./results.ppm"
def rayTracing(ray, scene):
    intersections = scene.find_intersections(ray)
    if not intersections:
        return background_color
    else:
        intersection, obj = find_nearser_object(ray, intersections)
        reflect_ray = obj.caculate_reflect_ray(ray, intersection)
        refract_ray = obj.caculate_refract_ray(ray, intersection)
        main_color = obj.radiance
        reflect_color = rayTracing(reflect_ray, scene)
        refract_color = rayTracing(refract_ray, scene)
        return mix(main_color, reflect_color, refract_color)

class Object(object):
    def __init__(self):
        self.radiance = 0
    def caculate_reflect_ray(self, ray, intersection):
        origin = intersection.position
        direction = ray.direction + 2 * intersection.y_element * intersection.normal
        return Ray(origin, direction)
    def caculate_refract_ray(self, ray, intersection):
        # TODO
        origin = intersection.position
        direction = ray.direction
        return Ray(origin, direction)

class Intersection(object):
    def __init__(self):
        pass

class Sphere(Object):
    def __init__(self, radius, position, emission, color, refl):
        self.radius = radius
        self.position = position
        self.emission = emission
        self.color = color
        self.refl = refl
    def find_intersection(self, ray):
        intersection = Intersection()
        return intersection


class Scene(object):
    def __init__(self, objects):
        self.objects = objects
    def find_intersections(self, ray):
        intersections = []
        for object in self.objects:
            intersection = object.find_intersection(ray)
            intersections.append(intersection)
        return intersections

class Ray(object):
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction


class Camera(object):
    def __init__(self, width, height, focal_length):
        self.width, self.height = width, height
        self.focal_length = focal_length
        self.colors = np.zeros([height, width, 3])
        self.origin = np.array([width/2, height/2, 0])
    def generate_ray(self, x, y):
        direction = np.array([x, y, self.focal_length]) - self.origin
        direction_norm = np.linalg.norm(direction)
        direction = direction / direction_norm
        ray = Ray(self.origin, direction)
        return ray


def mix(color1, color2, color3):
    return color1 + color2 + color3

def find_nearser_object(ray, intersections):
    intersection = Intersection()
    obj = Object()
    return intersection, obj

def write_to_file(colors, file_name):
    pass


if __name__ == "__main__":
    object1, object2, object3 = Object(), Object(), Object()
    scene = Scene([object1, object2, object3])
    camera = Camera(400, 400, 300)
    colors = camera.colors
    for x in range(camera.width):
        for y in range(camera.height):
            ray = camera.generate_ray(x, y)
            color = rayTracing(ray, scene)
            colors[x][y] = color
    write_to_file(colors, file_name)
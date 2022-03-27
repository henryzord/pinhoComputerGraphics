import argparse
import os
from functools import reduce

import numpy as np
import shapefile
from descartes import PolygonPatch
from matplotlib import animation
from matplotlib import pyplot as plt
from matplotlib.patches import Polygon as pltPolygon
from datetime import datetime as dt

from geometries import Polygon, Map


def times(map_path, method='map'):
    file_name = map_path.split(os.sep)[-1]
    read_path = os.path.join(map_path, file_name)
    geometry = shapefile.Reader(read_path, encoding='latin-1')

    descartes_poly = geometry.shapeRecords()[1].__geo_interface__
    abc = descartes_poly['geometry']['coordinates']

    while isinstance(abc[0][0], tuple):
        abc = abc[0]
    abc = list(abc)
    polygons_plot_data = np.array(abc).astype(np.float64)
    polygon = Polygon(abc)

    point = (-36.490942842432304, -9.750862586723057)

    myMap = Map(polygons=[polygon])

    func_dict = {
        'regular': polygon.isInside,
        'map': myMap.checkInside,
        'convexHull': polygon.toConvexHull().isInside
    }

    time_sum = {k: 0 for k in func_dict.keys()}

    n_repeat = 10000
    for k, func in func_dict.items():
        t1 = dt.now()
        for repeat in range(n_repeat):
            func(point)
        t2 = dt.now()
        time_sum[k] = (t2 - t1).total_seconds()

    print('times for different methods:')
    for k, time in time_sum.items():
        print('%s: %f' % (k, time))


def main(map_path):
    file_name = map_path.split(os.sep)[-1]
    read_path = os.path.join(map_path, file_name)
    geometry = shapefile.Reader(read_path, encoding='latin-1')

    fig, ax = plt.subplots()

    polygons_plot_data = []
    polygons_obj = []

    for i, sr in enumerate(geometry.shapeRecords()):
        descartes_poly = sr.__geo_interface__['geometry']
        if descartes_poly['type'] == 'Polygon':
            abc = list(descartes_poly['coordinates'][0])
            polygons_plot_data += [np.array(abc).astype(np.float64)]
            polygons_obj += [Polygon(abc)]
        elif descartes_poly['type'] == 'MultiPolygon':
            for subpol in descartes_poly['coordinates']:
                abc = list(subpol[0])
                polygons_plot_data += [np.array(abc).astype(np.float64)]
                polygons_obj += [Polygon(abc)]

    polygons_plot_data = np.array(polygons_plot_data)

    x_lim = [reduce(min, map(lambda x: min(x[:, 0]), polygons_plot_data)), reduce(max, map(lambda x: max(x[:, 0]), polygons_plot_data))]
    y_lim = [reduce(min, map(lambda x: min(x[:, 1]), polygons_plot_data)), reduce(max, map(lambda x: max(x[:, 1]), polygons_plot_data))]

    ax.set_xlim(*x_lim)
    ax.set_ylim(*y_lim)

    init_pos = np.array((x_lim[0] + 4, y_lim[0] + 3), dtype=np.float64)
    speed = np.array([0.15, 0.2], dtype=np.float64)

    myMap = Map(polygons=polygons_obj)

    # generates data for current time
    def data_gen():
        t = data_gen.t
        count = 0
        while count < 360:
            count += 15
            t += 1.0
            new_pos = init_pos
            new_pos[0] = init_pos[0] + (speed[0] * t)  # + ((0.01 * t)**2.)/2.
            new_pos[1] = init_pos[1] + (speed[1] * t)  # + ((0.01 * t) ** 2.) / 2.

            if new_pos[0] > max(x_lim):
                speed[0] *= -1
                new_pos[0] = max(x_lim)
            elif new_pos[0] < min(x_lim):
                speed[0] *= -1
                new_pos[0] = min(x_lim)

            if new_pos[1] > max(y_lim):
                speed[1] *= -1
                new_pos[1] = max(y_lim)
            elif new_pos[1] < min(y_lim):
                speed[1] *= -1
                new_pos[1] = min(y_lim)

            yield t, new_pos

    data_gen.t = 0

    # intialize two line objects (one in each axes)
    sct = ax.scatter([], [], lw=2)

    # the same axes initalizations as before (just now we do it for both of them)

    # ax.set_axis_off()

    # initialize the data arrays

    def run(data):
        # update the data
        t, p = data

        # inside_index = myMap.checkInside(p)
        # inside_index = -1

        patches = [ax.scatter(p[0], p[1], ec='black', c='white')]

        for i, original_data in enumerate(polygons_plot_data):
            inside_index = myMap.checkInside(tuple(p))
            if inside_index == i:
                pltpol0 = pltPolygon(original_data, fill=True, color='#88d2db', ec='black', alpha=0.4, zorder=0)
            else:
                pltpol0 = pltPolygon(original_data, fill=False, ec='black', alpha=0.4, zorder=0)

            patches += [ax.add_patch(pltpol0)]

        # ax.figure.canvas.draw()

        return patches

    ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=100, repeat=True)

    ax.axis('scaled')
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='script for collapsing metrics'
    )

    parser.add_argument(
        '--map-path', action='store', required=True,
        help='Path to a folder where shapefile map is stored.'
    )

    parser.add_argument(
        '--test-times', action='store_true', required=False, default=False,
        help='Whether to test times of methods instead of plotting the map to screen.'
    )

    args = parser.parse_args()

    if args.test_times:
        times(map_path=args.map_path)
    else:
        main(map_path=args.map_path)

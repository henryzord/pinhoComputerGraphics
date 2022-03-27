from geometries import Polygon, Map
from geometries import rotatePoint
from matplotlib import pyplot as plt
from matplotlib.patches import Polygon as pltPolygon
from matplotlib import animation
import numpy as np
from datetime import datetime as dt
from timeit import timeit


def envelope_collision_detection(axes: np.array, base_index: int):
    # plots envelope
    polA = Polygon([(2, 3), (3, 2), (4, 3)])
    envA = polA.getEnvelope()
    polB = Polygon([(2.75, 4), (2, 6), (4, 5)])
    envB = polB.getEnvelope()
    polC = Polygon([(2, 4.5), (3, 3.5), (4, 4.5)])
    envC = polC.getEnvelope()
    # envelope = np.array(envA)

    colorAB = '#e3a3d5' if polA.checkEnvelopeIntersection(polB) else '#88d2db'
    colorBC = '#e3a3d5' if polB.checkEnvelopeIntersection(polC) else '#88d2db'

    axes[base_index].add_patch(pltPolygon(np.array(polA.vertices)[:, :2].astype(np.float32), fill=True, color=colorAB, alpha=0.4))
    axes[base_index].add_patch(pltPolygon(np.array(envA)[:, :2].astype(np.float32), fill=False, color='black', alpha=0.4))
    axes[base_index].add_patch(pltPolygon(np.array(polB.vertices)[:, :2].astype(np.float32), fill=True, color=colorAB, alpha=0.4))
    axes[base_index].add_patch(pltPolygon(np.array(envB)[:, :2].astype(np.float32), fill=False, color='black', alpha=0.4))
    axes[base_index].set_xlim(0, 6)
    axes[base_index].set_ylim(0, 7)
    axes[base_index].set_axis_off()

    axes[base_index + 1].add_patch(pltPolygon(np.array(polB.vertices)[:, :2].astype(np.float32), fill=True, color=colorBC, alpha=0.4))
    axes[base_index + 1].add_patch(pltPolygon(np.array(envB)[:, :2].astype(np.float32), fill=False, color='black', alpha=0.4))
    axes[base_index + 1].add_patch(pltPolygon(np.array(polC.vertices)[:, :2].astype(np.float32), fill=True, color=colorBC, alpha=0.4))
    axes[base_index + 1].add_patch(pltPolygon(np.array(envC)[:, :2].astype(np.float32), fill=False, color='black', alpha=0.4))

    axes[base_index + 1].set_xlim(0, 6)
    axes[base_index + 1].set_ylim(0, 7)
    axes[base_index + 1].set_axis_off()

    return axes, base_index + 2


def map_insertion(axes: np.array, fig, base_index: int):

    axes[base_index].set_xlim(-1, 8)
    axes[base_index].set_ylim(-1, 8)

    point = (3, 2)
    pivot = (4, 3)

    myMap = Map(polygons=[
        Polygon([(0, 4), (3, 0), (4, 3)]),
        Polygon([(3, 0), (7, 2), (4, 3)]),
        Polygon([(4, 3), (7, 2), (7, 5)]),
        Polygon([(0, 4), (4, 3), (3, 7)]),
        Polygon([(4, 3), (3, 7), (7, 5)])
    ])

    polygons_plot_data = [np.array(pol.vertices)[:, :2].astype(np.float32) for pol in myMap.polygons]

    # generates data for current time
    def data_gen():
        t = data_gen.t
        count = 0
        while count < 360:
            count += 15
            t += 0.05
            new_point = rotatePoint(point, pivot, (3.14 * count)/180.0)
            yield t, new_point

    data_gen.t = 0

    # intialize two line objects (one in each axes)
    sct = axes[base_index].scatter([], [], lw=2)

    # the same axes initalizations as before (just now we do it for both of them)

    axes[base_index].set_axis_off()

    # initialize the data arrays

    def run(data):
        # update the data
        t, p = data

        # inside_index = myMap.checkInside(p)
        # inside_index = -1

        patches = [axes[base_index].scatter(p[0], p[1], ec='black', c='white')]

        for i, original_data in enumerate(polygons_plot_data):
            if myMap.checkInside(p) == i:
                pltpol0 = pltPolygon(original_data, fill=True, color='#88d2db', ec='black', alpha=0.4, zorder=0)
            else:
                pltpol0 = pltPolygon(original_data, fill=False, ec='black', alpha=0.4, zorder=0)

            patches += [axes[base_index].add_patch(pltpol0)]

        # axes[base_index].figure.canvas.draw()

        return patches

    ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=100, repeat=True)

    return axes, base_index + 1


def convex_hull(axes: np.array, base_index: int):

    concavos = [Polygon([(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)]), Polygon([(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)])]

    # testa convex hull
    for i, pol in enumerate(concavos):
        original_data = np.array(pol.vertices)[:, :2].astype(np.float32)
        convex_data = np.array(pol.toConvexHull().vertices)[:, :2].astype(np.float32)

        axes[base_index + i].set_xlim(min(original_data[:, 0]), max(original_data[:, 0]))
        axes[base_index + i].set_ylim(min(original_data[:, 1]), max(original_data[:, 1]))

        pltpol0 = pltPolygon(original_data, fill=False, ec='black', alpha=0.4, zorder=0)
        axes[base_index + i].add_patch(pltpol0)

        pltpol1 = pltPolygon(convex_data, fill=True, ec='#88d2db', alpha=0.4, zorder=1)
        axes[base_index + i].add_patch(pltpol1)

        axes[base_index + i].set_axis_off()

    return axes, base_index + len(concavos)


def points_inside_polygons(axes: np.array, base_index: int):
    """
    Testa se um ponto está dentro de polígono côncavo/convexo.

    :param axes: Onde projetar os gráficos
    :param base_index: o menor índice dos gráficos a ser usado
    :rtype: tuple
    :return: Uma tupla onde o primeiro membro é axes atualizado e o segundo o próximo índice utilizável
    """

    testing_points = [(1, 5), (5, 1), (8, 5), (6, 4), (4, 3)]

    convexos = [Polygon([(5, 0), (3, 2), (5, 6), (7, 3)]), Polygon([(2, 0), (2, 6), (7, 6), (7, 0)])]
    corretude_convexos = [[0, 1, 0, 1, 1], [0, 1, 0, 1, 1]]
    concavos = [Polygon([(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)]), Polygon([(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)])]
    corretude_concavos = [[0, 0, 0, 1, 1], [0, 0, 0, 1, 1]]

    for i, convex in enumerate(convexos):
        pltpol = pltPolygon(np.array(convex.vertices)[:, :2].astype(np.float32), fill=False, ec='black', alpha=0.4)
        axes[base_index + i].add_patch(pltpol)
        axes[base_index + i].set_axis_off()

        for t_point, rightfulness in zip(testing_points, corretude_convexos[i]):
            axes[base_index + i].scatter(
                t_point[0], t_point[1], c='#88d2db' if convex.isInside(t_point) == bool(rightfulness) else '#e3a3d5'
            )

    for i, concav in enumerate(concavos):
        pltpol = pltPolygon(np.array(concav.vertices)[:, :2].astype(np.float32), fill=False, ec='black', alpha=0.4)
        axes[base_index + len(convexos) + i].add_patch(pltpol)
        axes[base_index + len(convexos) + i].set_axis_off()

        for t_point, rightfulness in zip(testing_points, corretude_concavos[i]):
            axes[base_index + len(convexos) + i].scatter(
                t_point[0], t_point[1], c='#88d2db' if concav.isInside(t_point) == bool(rightfulness) else '#e3a3d5'
            )

    return axes, base_index + len(convexos) + len(concavos)





def main():
    fig, axes = plt.subplots(nrows=3, ncols=3)
    axes = np.ravel(axes)

    base_index = 0
    axes, base_index = points_inside_polygons(axes, base_index=base_index)
    axes, base_index = convex_hull(axes, base_index=base_index)
    axes, base_index = envelope_collision_detection(axes, base_index=base_index)
    axes, base_index = map_insertion(axes, fig=fig, base_index=base_index)

    plt.show()


if __name__ == '__main__':
    main()

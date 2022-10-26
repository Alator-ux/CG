#pragma once
#include "Figure.h"
#include "STLLoader.h"
class FigureBuilder {
    float bound = 100;
public:
    Figure buildFigure(FigureType kind, glm::vec3 color) {
        switch (kind) {
        case Tetrahed:
            return buildTetrahedron(color);
        case Hexahed:
            return buildHexahedron(color);
        case Octahed:
            return buildOctahedron(color);
        case Icosahed:
            return buildIcosahedron(color);
        case Dodecahed:
            break;
        }
    }

    Tetrahedron buildTetrahedron(glm::vec3 color) {
        auto tetr = Tetrahedron();
        glm::vec3 a = glm::vec3(0, 0, 0);
        glm::vec3 c = glm::vec3(bound / 2, 0, -bound / 2);
        glm::vec3 f = glm::vec3(bound / 2, bound / 2, 0);
        glm::vec3 h = glm::vec3(0, bound / 2, -bound / 2);
        a /= bound;
        c /= bound;
        f /= bound;
        h /= bound;

        Face face1 = Face(a, glm::vec3(1.f, 0.f, 0.f));
        face1.push_point(f);
        face1.push_point(c);
        face1.primitive_is_finished();

        Face face2 = Face(f, glm::vec3(0.f, 1.f, 0.f));
        face2.push_point(c);
        face2.push_point(h);
        face2.primitive_is_finished();

        Face face3 = Face(c, glm::vec3(0.f, 0.f, 1.f));
        face3.push_point(h);
        face3.push_point(a);
        face3.primitive_is_finished();

        Face face4 = Face(f, color);
        face4.push_point(h);
        face4.push_point(a);
        face4.primitive_is_finished();

        tetr.faces.push_back(face1);
        tetr.faces.push_back(face2);
        tetr.faces.push_back(face3);
        tetr.faces.push_back(face4);
        return tetr;

    }

    Hexahedron buildHexahedron(glm::vec3 color) {
        auto hexa = Hexahedron();
        glm::vec3 a = glm::vec3(0, 0, 0);
        glm::vec3 b = glm::vec3(bound / 2, 0, 0);
        glm::vec3 c = glm::vec3(bound / 2, 0, -bound / 2);
        glm::vec3 d = glm::vec3(0, 0, -bound / 2);
        glm::vec3 e = glm::vec3(0, bound / 2, 0);
        glm::vec3 f = glm::vec3(bound / 2, bound / 2, 0);
        glm::vec3 g = glm::vec3(bound / 2, bound / 2, -bound / 2);
        glm::vec3 h = glm::vec3(0, bound / 2, -bound / 2);
        a /= bound;
        b /= bound;
        c /= bound;
        d /= bound;
        e /= bound;
        f /= bound;
        g /= bound;
        h /= bound;

        Face face1 = Face(a, glm::vec3(1.f, 0.f, 0.f));
        face1.push_point(b);
        face1.push_point(c);
        face1.push_point(d);
        face1.primitive_is_finished();

        Face face2 = Face(b, glm::vec3(0.f, 1.f, 0.f));
        face2.push_point(c);
        face2.push_point(g);
        face2.push_point(f);
        face2.primitive_is_finished();

        Face face3 = Face(f, glm::vec3(0.f, 0.f, 1.f));
        face3.push_point(g);
        face3.push_point(h);
        face3.push_point(e);
        face3.primitive_is_finished();

        Face face4 = Face(h, glm::vec3(1.f, 1.f, 0.f));
        face4.push_point(e);
        face4.push_point(a);
        face4.push_point(d);
        face4.primitive_is_finished();

        Face face5 = Face(a, glm::vec3(1.f, 0.f, 1.f));
        face5.push_point(b);
        face5.push_point(f);
        face5.push_point(e);
        face5.primitive_is_finished();


        Face face6 = Face(d, glm::vec3(0.f, 1.f, 1.f));
        face6.push_point(c);
        face6.push_point(g);
        face6.push_point(h);
        face6.primitive_is_finished();

        hexa.faces.push_back(face1);
        hexa.faces.push_back(face2);
        hexa.faces.push_back(face3);
        hexa.faces.push_back(face4);
        hexa.faces.push_back(face5);
        hexa.faces.push_back(face6);

        return hexa;
    }

    Octahedron buildOctahedron(glm::vec3 color) {
        auto res = Octahedron();
        auto core = buildHexahedron(color);
        auto a = core.faces[0].center();
        auto b = core.faces[1].center();
        auto c = core.faces[2].center();
        auto d = core.faces[3].center();
        auto e = core.faces[4].center();
        auto f = core.faces[5].center();

        Face face1 = Face(a, glm::vec3(1.f, 0.f, 0.f));
        face1.push_point(f);
        face1.push_point(b);
        face1.primitive_is_finished();

        Face face2 = Face(b, glm::vec3(0.f, 1.f, 0.f));
        face2.push_point(c);
        face2.push_point(f);
        face2.primitive_is_finished();

        Face face3 = Face(c, glm::vec3(0.f, 0.f, 1.f));
        face3.push_point(d);
        face3.push_point(f);
        face3.primitive_is_finished();

        Face face4 = Face(d, glm::vec3(1.f, 1.f, 0.f));
        face4.push_point(a);
        face4.push_point(f);
        face4.primitive_is_finished();

        Face face5 = Face(a, glm::vec3(1.f, 0.f, 1.f));
        face5.push_point(e);
        face5.push_point(b);
        face5.primitive_is_finished();

        Face face6 = Face(b, glm::vec3(0.f, 1.f, 1.f));
        face6.push_point(e);
        face6.push_point(c);
        face6.primitive_is_finished();

        Face face7 = Face(c, glm::vec3(1.f, 1.f, 1.f));
        face7.push_point(e);
        face7.push_point(d);
        face7.primitive_is_finished();

        Face face8 = Face(d, glm::vec3(0.5f, 0.5f, 0.5f));
        face8.push_point(e);
        face8.push_point(a);
        face8.primitive_is_finished();

        res.faces.push_back(face1);
        res.faces.push_back(face2);
        res.faces.push_back(face3);
        res.faces.push_back(face4);
        res.faces.push_back(face5);
        res.faces.push_back(face6);
        res.faces.push_back(face7);
        res.faces.push_back(face8);
        return res;
    }

    Icosahedron buildIcosahedron(glm::vec3 color) {
        auto r = STL::load_from_file("./models/icosahedron.stl");
        Icosahedron* res = reinterpret_cast<Icosahedron*>(&r);
        return *res;
        //        auto res = Icosahedron();
                /*auto an_color = glm::vec3(1.f) - color;
                auto half_bound = bound / 2;
                float r = half_bound;
                auto p1 = glm::vec3(0, -half_bound, -r) / bound;
                auto p2 = glm::vec3(0, half_bound, -r) / bound;
                auto p3 = glm::vec3(half_bound, r, 0) / bound;
                auto p4 = glm::vec3(r, 0, -half_bound) / bound;
                auto p5 = glm::vec3(half_bound, -r, 0) / bound;
                auto p6 = glm::vec3(-half_bound, -r, 0) / bound;
                auto p7 = glm::vec3(-r, 0, -half_bound) / bound;
                auto p8 = glm::vec3(-half_bound, r, 0) / bound;
                auto p9 = glm::vec3(r, 0, half_bound) / bound;
                auto p10 = glm::vec3(-r, 0, half_bound) / bound;
                auto p11 = glm::vec3(0, -half_bound, r) / bound;
                auto p12 = glm::vec3(0, half_bound, r) / bound;

                Face face = Face(p1, glm::vec3(1.f, 0.f, 0.f));
                face.push_point(p2);
                face.push_point(p4);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p1, glm::vec3(0.f, 1.f, 0.f));
                face.push_point(p2);
                face.push_point(p7);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p7, glm::vec3(0.f, 0.f, 1.f));
                face.push_point(p2);
                face.push_point(p8);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p8, glm::vec3(1.f, 1.f, 0.f));
                face.push_point(p2);
                face.push_point(p3);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p4, glm::vec3(1.f, 0.f, 1.f));
                face.push_point(p2);
                face.push_point(p3);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p6, glm::vec3(0.f, 1.f, 1.f));
                face.push_point(p1);
                face.push_point(p5);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p6, glm::vec3(1.f, 1.f, 1.f));
                face.push_point(p7);
                face.push_point(p10);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p10, glm::vec3(0.f, 0.f, 0.f));
                face.push_point(p7);
                face.push_point(p8);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p10, glm::vec3(0.5f, 0.f, 0.f));
                face.push_point(p8);
                face.push_point(p12);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p12, glm::vec3(0.5f, 0.5f, 0.f));
                face.push_point(p8);
                face.push_point(p3);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p9, glm::vec3(0.5f, 0.0f, 0.5f));
                face.push_point(p4);
                face.push_point(p3);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p5, glm::vec3(0.0f, 0.5f, 0.5f));
                face.push_point(p4);
                face.push_point(p9);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p12, glm::vec3(0.5f, 0.5f, 0.5f));
                face.push_point(p3);
                face.push_point(p9);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p5, glm::vec3(1.0f, 0.5f, 0.5f));
                face.push_point(p1);
                face.push_point(p4);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p7, glm::vec3(1.0f, 0.5f, 0.5f));
                face.push_point(p1);
                face.push_point(p6);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p11, glm::vec3(1.0f, 1.0f, 0.5f));
                face.push_point(p5);
                face.push_point(p6);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p11, glm::vec3(1.0f, 0.5f, 1.0f));
                face.push_point(p6);
                face.push_point(p10);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p11, glm::vec3(0.5f, 1.0f, 1.0f));
                face.push_point(p10);
                face.push_point(p12);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p11, glm::vec3(0.0f, 0.5f, 0.5f));
                face.push_point(p12);
                face.push_point(p9);
                face.primitive_is_finished();
                res.faces.push_back(face);

                face = Face(p11, glm::vec3(0.5f, 0.0f, 0.5f));
                face.push_point(p5);
                face.push_point(p9);
                face.primitive_is_finished();
                res.faces.push_back(face);

                return res;*/
    }
};
def can_build(env, platform):
    return not env["disable_3d"]


def configure(env):
    pass


def get_doc_classes():
    return [
        "SEOBox3D",
        "SEOCombiner3D",
        "SEOCylinder3D",
        "SEOMesh3D",
        "SEOPolygon3D",
        "SEOPrimitive3D",
        "SEOShape3D",
        "SEOSphere3D",
        "SEOTorus3D",
    ]


def get_doc_path():
    return "doc_classes"

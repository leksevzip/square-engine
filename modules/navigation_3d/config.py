def can_build(env, platform):
    if env["disable_navigation_3d"]:
        return False

    env.module_add_dependencies("navigation", ["SEO", "gridmap"], True)
    return True


def configure(env):
    pass

#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});
    glEnable(GL_DEPTH_TEST);
    

    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};
    
    auto const shaderRetangle = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex2.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment2.glsl"},
    }};

    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}, gl::VertexAttribute::UV{1}},
            .data   = {
                -1.f, -1.f, 0., 0.,
                +1.f, -1.f, 1., 0.,
                +1.f, +1.f, 1., 1.,
                -1.f, +1.f, 0., 1.,
            },
        }},
        .index_buffer   = {
            0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
            0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
        },
    }};


    auto const cube_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}},
            .data   = {
                -0.5f, -0.5f, -0.5f, 0., 0.,
                -0.5f, -0.5f, +0.5f, 0., 1.,
                -0.5f, +0.5f, -0.5f, 1., 1., 
                -0.5f, +0.5f, +0.5f, 1., 0.,
                +0.5f, -0.5f, -0.5f, 0., 0.,
                +0.5f, -0.5f, +0.5f, 0., 1.,
                +0.5f, +0.5f, -0.5f, 1., 1.,
                +0.5f, +0.5f, +0.5f, 1., 0.,
            },
        }},

        .index_buffer   = {
            0, 1, 2, 
            1, 2, 3,
            0, 4, 2,
            4, 2, 6,
            2, 6, 3,
            3, 6, 7,
            0, 1, 4,
            1, 4, 5,
            1, 3, 5,
            3, 5, 7,
            6, 4, 5,
            5, 6, 7,
        },
    }};

    auto const texture = gl::Texture{
    gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
        .path           = "res/texture-2b39c87c6a92ef42086e1627d4339ade.png",
        .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
        .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
    },
    gl::TextureOptions{
        .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
        .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
        .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
        .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
    }};

    auto render_target = gl::RenderTarget{gl::RenderTarget_Descriptor{
    .width          = gl::framebuffer_width_in_pixels(),
    .height         = gl::framebuffer_height_in_pixels(),
    .color_textures = {
        gl::ColorAttachment_Descriptor{
            .format  = gl::InternalFormat_Color::RGBA8,
            .options = {
                .minification_filter  = gl::Filter::NearestNeighbour, // On va toujours afficher la texture à la taille de l'écran,
                .magnification_filter = gl::Filter::NearestNeighbour, // donc les filtres n'auront pas d'effet. Tant qu'à faire on choisit le moins coûteux.
                .wrap_x               = gl::Wrap::ClampToEdge,
                .wrap_y               = gl::Wrap::ClampToEdge,
            },
        },

        
    },
    .depth_stencil_texture = gl::DepthStencilAttachment_Descriptor{
        .format  = gl::InternalFormat_DepthStencil::Depth32F,
        .options = {
            .minification_filter  = gl::Filter::NearestNeighbour,
            .magnification_filter = gl::Filter::NearestNeighbour,
            .wrap_x               = gl::Wrap::ClampToEdge,
            .wrap_y               = gl::Wrap::ClampToEdge,
        },
    },
    }};

    gl::set_events_callbacks({
    camera.events_callbacks(),
    {.on_framebuffer_resized = [&](gl::FramebufferResizedEvent const& e) {
        if(e.width_in_pixels != 0 && e.height_in_pixels != 0) // OpenGL crash si on tente de faire une render target avec une taille de 0
            render_target.resize(e.width_in_pixels, e.height_in_pixels);
    }},
    });

    while (gl::window_is_open())
    {
         render_target.render([&]() {
            // Rendu à chaque frame
            glClearColor(1.f, 0.f, 0.f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target

            glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Vient remplacer glClear(GL_COLOR_BUFFER_BIT);
            glm::mat4 const view_matrix = camera.view_matrix();
            glm::mat4 const projection_matrix = glm::infinitePerspective(glm::radians(45.f) /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
            //glm::mat4 const translation = glm::translate(glm::mat4{1.f}, glm::vec3{1.f, 0.f, 0.f} /* déplacement */);
            //glm::mat4 const model_matrix = translation ;
            glm::mat4 const view_projection_matrix = projection_matrix * view_matrix;
            //glm::mat4 const rotation = glm::rotate(glm::mat4{1.f}, gl::time_in_seconds() /*angle de la rotation*/, glm::vec3{0.f, 0.f, 1.f} /* axe autour duquel on tourne */);
            //glm::mat4 const model_view_projection_matrix = view_projection_matrix * model_matrix;
        
            shader.bind(); 
            shader.set_uniform("aspect_ratio", glm::float32{gl::framebuffer_aspect_ratio()});
            shader.set_uniform("Time", glm::float32{gl::time_in_seconds()});
            shader.set_uniform("view_projection_matrix", view_projection_matrix);
            shader.set_uniform("my_texture", texture);

            //rectangle_mesh.draw(); 
            cube_mesh.draw();
        });

        glClear(GL_DEPTH_BUFFER_BIT);
        shaderRetangle.bind();
        shaderRetangle.set_uniform("my_texture", render_target.color_texture(0) );
        rectangle_mesh.draw();
    }
}
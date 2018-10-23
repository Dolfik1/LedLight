module.exports = function(grunt) {
    grunt.initConfig({
        smoosher: {
            options: {
              jsTags: {
                start: '<script type="text/javascript">',
                end: "</script>"
              },
            },
            all: {
              files: {
                "dist/index.html": "src/index.html",
              },
            },
        },
        htmlmin: {
            main: {
              options: {
                removeComments: true,
                collapseWhitespace: true,
                minifyJS: {
                    compress: {
                        sequences: true,
                        dead_code: true,
                        conditionals: true,
                        booleans: true,
                        unused: true,
                        if_return: true,
                        join_vars: true,
                        drop_console: true
                      },
                      mangle: {
                        except: ['$super', '$', 'exports', 'require']
                      },
                      output: {
                        comments: false
                    } 
                },
                minifyCSS: true,
                processScripts: [ "text/javascript" ]
              },
              files: {
                "dist/index.html": "dist/index.html",
              }
            }
          }
    });

    grunt.loadNpmTasks("grunt-html-smoosher");
    grunt.loadNpmTasks("grunt-contrib-htmlmin");
    grunt.registerTask("default", ["smoosher", "htmlmin"]);
}
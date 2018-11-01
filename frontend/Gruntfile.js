module.exports = function(grunt) {
    grunt.initConfig({
        htmlmin: {
          main: {
            options: {
              removeComments: true,
              collapseWhitespace: true
            },
            files: {
              "dist/index.html": "src/index.html"
            }
          }
        },
        cssmin: {
          options: {
            mergeIntoShorthands: false,
            roundingPrecision: -1
          },
          target: {
            files: {
              "dist/styles.css": "src/styles.css"
            }
          }
        },
        uglify: {
          main: {
            files: {
              "dist/app.js": "src/app.js"
            }
          }
        },
        compress: {
          main: {
            options: {
              mode: "gzip"
            },
            expand: true,
            cwd: "dist/",
            src: ['**/*'],
            dest: "dist/"
          }
        }
    });

    grunt.loadNpmTasks("grunt-contrib-htmlmin");
    grunt.loadNpmTasks("grunt-contrib-uglify");
    grunt.loadNpmTasks("grunt-contrib-cssmin");
    grunt.loadNpmTasks("grunt-contrib-compress");
    grunt.registerTask("default", ["htmlmin", "cssmin", "uglify", "compress"]);
}
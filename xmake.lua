add_rules("mode.debug", "mode.release")

target("gx")
    set_kind("binary")
    set_warnings("all", "extra", "pedantic")
    if is_os("linux") then
        set_languages("gnu11") -- I need GNU11 for clock_gettime function
    end
    add_files("gx.c", "src/*.c")
    add_includedirs("src")
 
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})


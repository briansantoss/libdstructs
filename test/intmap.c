#include "test.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map/intmap.h"

TEST(new) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);

    ASSERT_TRUE(intmap_is_empty(map));
    ASSERT_TRUE(intmap_size(map) == 0);
}

TEST(insert) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);

    int value;
    ASSERT_TRUE(intmap_insert(map, "a", 1) && intmap_has_key(map, "a"));
    ASSERT_TRUE(intmap_size(map) == 1);
    ASSERT_TRUE(intmap_get(map, "a", &value) && value == 1);
    ASSERT_FALSE(intmap_insert(map, "a", 2));
    
    ASSERT_TRUE(intmap_insert(map, "key", 55) && intmap_has_key(map, "key"));
    ASSERT_TRUE(intmap_size(map) == 2);
    ASSERT_TRUE(intmap_get(map, "key", &value) && value == 55);
    ASSERT_FALSE(intmap_insert(map, "key", 2));

    ASSERT_FALSE(intmap_insert(NULL, "null", 10));
    ASSERT_FALSE(intmap_insert(NULL, NULL, 45));

    // Insertion with a NULL key should fail silently and not alter the map
    ASSERT_FALSE(intmap_insert(map, NULL, 108) && intmap_has_key(map, NULL) );
    ASSERT_TRUE(intmap_size(map) == 2);
}

TEST(set) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);
    
    int value;
    ASSERT_TRUE(intmap_set(map, "new", 5) && intmap_has_key(map, "new"));
    ASSERT_TRUE(intmap_size(map) == 1);
    ASSERT_TRUE(intmap_get(map, "new", &value) && value == 5);
    
    ASSERT_TRUE(intmap_set(map, "new", 10)); 
    ASSERT_TRUE(intmap_get(map, "new", &value) && value == 10);
    ASSERT_TRUE(intmap_size(map) == 1);
    
    ASSERT_FALSE(intmap_set(NULL, "a", 56));
    ASSERT_FALSE(intmap_set(map, NULL, 190) && intmap_size(map) == 1);
}

TEST(remove) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);

    ASSERT_TRUE(intmap_insert(map, "1", 123));
    ASSERT_TRUE(intmap_insert(map, "2", 1980));
    ASSERT_TRUE(intmap_size(map) == 2);

    intmap_remove(map, "2");
    ASSERT_TRUE(intmap_size(map) == 1);
    ASSERT_FALSE(intmap_has_key(map, "2"));

    int value;
    ASSERT_TRUE(intmap_has_key(map, "1"));
    ASSERT_TRUE(intmap_get(map, "1", &value) && value == 123);

    intmap_remove(map, "not exists");
    ASSERT_TRUE(intmap_size(map) == 1);
}

TEST(clear) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);
    
    ASSERT_TRUE(intmap_insert(map, "a", 1));
    ASSERT_TRUE(intmap_insert(map, "b", 2));
    ASSERT_FALSE(intmap_is_empty(map));
    ASSERT_TRUE(intmap_size(map) > 0);
    ASSERT_TRUE(intmap_has_key(map, "a") && intmap_has_key(map, "b"));
    
    intmap_clear(map);
    ASSERT_TRUE(intmap_is_empty(map) && intmap_size(map) == 0);
    ASSERT_FALSE(intmap_has_key(map, "a") && intmap_has_key(map, "b"));
    
    ASSERT_TRUE(intmap_insert(map, "a", 234));
    ASSERT_TRUE(intmap_size(map) == 1);
    
    int value;
    ASSERT_TRUE(intmap_get(map, "a", &value) && value == 234);
}

TEST(equals) {
    IntMap m1 = intmap_new();
    IntMap m2 = intmap_new();
    ASSERT_TRUE(m1 && m2);
    
    ASSERT_TRUE(intmap_is_empty(m1) && intmap_is_empty(m2));
    ASSERT_TRUE(intmap_equals(m1, m2));
    
    ASSERT_TRUE(intmap_insert(m1, "x", 10));
    ASSERT_TRUE(intmap_insert(m2, "x", 10));
    ASSERT_TRUE(intmap_equals(m1, m2));
    
    ASSERT_TRUE(intmap_set(m2, "x", 20));
    ASSERT_FALSE(intmap_equals(m1, m2));
    
    ASSERT_TRUE(intmap_set(m1, "x", 20));
    ASSERT_TRUE(intmap_equals(m1, m2));
    
    // Inserting the same key-value pairs in different order should still result in equal maps
    ASSERT_TRUE(intmap_insert(m1, "z", 90) && intmap_insert(m2, "m", 70));
    ASSERT_TRUE(intmap_insert(m1, "m", 70) && intmap_insert(m2, "z", 90));
    ASSERT_TRUE(intmap_equals(m1, m2));
    
    ASSERT_TRUE(intmap_insert(m1, "new key", 30));
    ASSERT_FALSE(intmap_equals(m1, m2));
    
    ASSERT_FALSE(intmap_equals(NULL, NULL));
    ASSERT_FALSE(intmap_equals(m1, NULL));
    ASSERT_FALSE(intmap_equals(NULL, m2));
}

TEST(keys_and_values) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);
    
    char** keys = intmap_keys(map);
    int* values = intmap_values(map);
    ASSERT_FALSE(keys && values);
    
    ASSERT_TRUE(intmap_insert(map, "one", 1));
    ASSERT_TRUE(intmap_insert(map, "two", 2));
    
    keys = intmap_keys(map);
    values = intmap_values(map);
    ASSERT_TRUE(keys && values);
    ASSERT_TRUE(intmap_size(map) == 2);
    
    // Check if the key-value pairs match
    int found[2] = {0};
    for (int i = 0; i < 2; i++) {
        if (strcmp(keys[i], "one") == 0 && values[i] == 1) found[0] = 1;
        if (strcmp(keys[i], "two") == 0 && values[i] == 2) found[1] = 1;
    }
    ASSERT_TRUE(found[0] && found[1]);
    
    keys = intmap_keys(NULL);
    values = intmap_values(NULL);
    ASSERT_FALSE(keys && values);
}

TEST(resize) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);

    for (int i = 0; i < 1000; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        ASSERT_TRUE(intmap_insert(map, key, i));
    }

    for (int i = 0; i < 1000; i++) {
        char key[16];
        int value;
        sprintf(key, "key%d", i);
        ASSERT_TRUE(intmap_get(map, key, &value));
        ASSERT_TRUE(value == i);
    }

    ASSERT_TRUE(intmap_size(map) == 1000);
}

TEST(next) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);
    
    ASSERT_TRUE(intmap_is_empty(map));
    ASSERT_FALSE(intmap_iter_new(map));
    
    ASSERT_TRUE(intmap_insert(map, "A", 65));
    ASSERT_TRUE(intmap_insert(map, "B", 980));
    ASSERT_TRUE(intmap_insert(map, "C", 90));
    
    IntMapIter iter = intmap_iter_new(map);
    ASSERT_TRUE(iter);
    
    char* expected_keys = "ABC";
    int expected_values[] = {65, 980, 90};
    bool found[3] = {false, false, false};
    
    KeyValuePair pair;
    size_t index;
    
    for (int i = 0; i < 3; i++) {
        ASSERT_TRUE(intmap_iter_next(iter, &pair));
        index = strcspn(expected_keys, pair.key);
        ASSERT_TRUE(index != strlen(expected_keys));
        ASSERT_TRUE(!found[index]);
        found[index] = true;
        ASSERT_TRUE(pair.value == expected_values[index]);
    }
    
    ASSERT_FALSE(intmap_iter_next(iter, &pair));
    ASSERT_FALSE(intmap_iter_new(NULL));
}

TEST(reset) {
    IntMap map = intmap_new();
    ASSERT_TRUE(map);
    
    ASSERT_TRUE(intmap_insert(map, "A", 8909));
    ASSERT_TRUE(intmap_insert(map, "B", 90922));
    ASSERT_TRUE(intmap_insert(map, "C", 0));
    
    IntMapIter iter = intmap_iter_new(map);
    ASSERT_TRUE(iter);

    KeyValuePair pair;
    // Checks if the iterator could be used normally after reset
    for (int i = 0; i < 2; i++) {
        for (int i = 0; i < 3; i++) ASSERT_TRUE(intmap_iter_next(iter, &pair));
        ASSERT_FALSE(intmap_iter_next(iter, &pair));
        intmap_iter_reset(iter);
    }
}

int main() {
    TestCase tests[] = {
        {"new", test_new},
        {"insert", test_insert},
        {"set", test_set},
        {"remove", test_remove},
        {"clear", test_clear},
        {"equals", test_equals},
        {"keys and values", test_keys_and_values},
        {"resize", test_resize},
        {"next", test_next},
        {"reset", test_reset},
    };

    TestSuite suite = {.name = "IntMap", .tests = tests, .tests_num = sizeof (tests) / sizeof (tests[0])};

    run_suite_tests(&suite);
    return 0;
}
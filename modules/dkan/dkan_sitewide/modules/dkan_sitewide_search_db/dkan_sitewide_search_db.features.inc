<?php
/**
 * @file
 * dkan_sitewide_search_db.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dkan_sitewide_search_db_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dkan_sitewide_search_db_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_search_api_index().
 */
function dkan_sitewide_search_db_default_search_api_index() {
  $items = array();
  $items['datasets'] = entity_import('search_api_index', '{
    "name" : "datasets",
    "machine_name" : "datasets",
    "description" : null,
    "server" : "solr_localhost",
    "item_type" : "node",
    "options" : {
      "datasource" : { "bundles" : [
          "dataset",
          "resource",
          "group",
          "dkan_data_story",
          "data_dashboard",
          "page",
          "harvest_source"
        ]
      },
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "body:value" : { "type" : "text", "boost" : "2.0" },
        "changed" : { "type" : "date" },
        "created" : { "type" : "date" },
        "field_author" : { "type" : "text" },
        "field_dcatap_license" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_dcatap_license_dataset" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "field_dcatap_rightsholder:first" : { "type" : "text", "boost" : "2.0" },
        "field_dcatap_rightsholder:second" : { "type" : "text", "boost" : "3.0" },
        "field_harvester_reference:title" : { "type" : "text" },
        "field_resources:field_format" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_resources:title" : { "type" : "list\\u003Ctext\\u003E" },
        "field_tags" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_topic" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "nid" : { "type" : "integer" },
        "og_group_ref" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "search_api_access_node" : { "type" : "list\\u003Cstring\\u003E" },
        "search_api_language" : { "type" : "string" },
        "status" : { "type" : "boolean" },
        "title" : { "type" : "text", "boost" : "5.0" },
        "type" : { "type" : "string" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_node_status" : { "status" : 1, "weight" : "-50", "settings" : [] },
        "search_api_alter_node_access" : { "status" : 1, "weight" : "-49", "settings" : [] },
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-48",
          "settings" : { "default" : "0", "bundles" : { "dataset" : "dataset" } }
        },
        "search_api_metatag_alter_callback" : { "status" : 0, "weight" : "-47", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "-46", "settings" : [] },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "-45", "settings" : [] },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "-44", "settings" : { "mode" : "full" } },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "-43", "settings" : { "fields" : [] } },
        "search_api_alter_language_control" : {
          "status" : 0,
          "weight" : "-42",
          "settings" : { "lang_field" : "", "languages" : [] }
        },
        "panelizer" : { "status" : 0, "weight" : "-41", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "fields" : { "body:value" : true, "field_resources:body:value" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : { "fields" : [], "spaces" : "[^[:alnum:]]", "ignorable" : "[\\u0027]" }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : [],
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "35",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function dkan_sitewide_search_db_default_search_api_server() {
  $items = array();
  $items['datasets'] = entity_import('search_api_server', '{
    "name" : "datasets",
    "machine_name" : "datasets",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "2",
      "partial_matches" : 0,
      "indexes" : []
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}

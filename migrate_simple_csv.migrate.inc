<?php

/**
 * @file
 *   A baseball game migration example.
 */


/*
 * Implement hook_migrate_api()
 */
function migrate_example_baseball_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'simple_csv' => array(
        'title' => t('Simple CSV import to Basic Page'),
      ),
    ),
  );
}

/**
 * A migration that is reused for each source CSV file.
 */
class SimpleCSV extends Migration 
  public function __construct($arguments) {

  }

  protected function csvcolumns() {
  
  }

  public function prepareRow($row) {
  
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    return drupal_strtolower(pathinfo($this->arguments['source_file'],
                                      PATHINFO_FILENAME));
  }
  
}
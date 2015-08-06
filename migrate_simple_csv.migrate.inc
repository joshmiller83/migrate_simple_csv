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
    // Run the parent first
    parent::__construct($arguments);
    $this->description = t('Import 10 simple nodes from CSV file.');

    // Create a map object for tracking the relationships between source rows
    // Only tracking the UUID and the node id relationship.
    // arg 1: Machine Name
    // arg 2: Source Key
    // arg 3: Destination Key
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uuid' => array('type' => 'int',
                         'size' => 'normal',
                         'not null' => TRUE,
                         'description' => 'uuid',
                        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($arguments['source_file'],
                                         $this->csvcolumns(),
                                         array(),
                                         $this->fields());

    // Add destination to standard basic page content type
    $this->destination = new MigrateDestinationNode('page');

    $this->addFieldMapping(NULL, 'uuid')
         ->description('Simple unique id for tracking once imported.');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'description-en');
    $this->addFieldMapping(NULL, 'description-fr')
         ->description('Not needed yet.');
  }

  protected function csvcolumns() {
    $columns[0] = array('start_date', 'Date of game');
    $columns[1] = array('visiting_team', 'Visiting team');

    return $columns;
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

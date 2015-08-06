<?php

/**
 * @file
 *   A csv simple importer for Josh to learn.
 */


/**
 * Implements hook_migrate_api()
 *
 * Returns 'api' => 2 for the 7.x-2.x branch of Migrate.
 * Registers the migration classes for the 7.x-2.6 branch of Migrate (including
 * 7.x-2.6+xx-dev).
 */
function migrate_simple_csv_migrate_api() {
  $api = array(
    'api' => 2,
    // register a group
    'groups' => array(
      'simple_csv' => array(
        'title' => t('Simple CSV import to Basic Page'),
      ),
    ),
    'migrations' => array(
      'SimpleCSVenMigrate' => array(
        'class_name' => 'SimpleCSVenMigrate',
        'group_name' => 'simple_csv',
      ),
      'SimpleCSVfrMigrate' => array(
        'class_name' => 'SimpleCSVfrMigrate',
        'group_name' => 'simple_csv',
      ),
    ),
  );
  return $api;
}

/**
 * Abstract class as a base for all our migration classes.
 */
class SimpleCSVBasicMigration extends Migration {
  public function __construct($arguments) {
    // Always call the parent constructor first for basic setup
    parent::__construct($arguments);

    // Avoid known line ending issue: "Invalid data value" at drupal.org/node/1152158#InvalidDataValue
    ini_set('auto_detect_line_endings', TRUE);


    $this->description = t('Import 10 simple nodes from CSV file.');

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($arguments['source_file'],
                                         $this->csvcolumns(),
                                         array("header_rows" => 1),
                                         $this->fields());

    // Add destination to standard basic page content type
    $this->destination = new MigrateDestinationNode('page');

    $this->addFieldMapping(NULL, 'UUID')
         ->description('Simple unique id for tracking once imported.');
    $this->addFieldMapping('title', 'Title');
  }

  protected function csvcolumns() {
    $columns[0] = array('UUID', 'UUID');
    $columns[1] = array('Title', 'Title');
    $columns[2] = array('description-en', 'description-en');
    $columns[3] = array('description-fr', 'description-fr');

    return $columns;
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    return drupal_strtolower(pathinfo($this->arguments['source_file'],
                                      PATHINFO_FILENAME));
  }

  /* Add fields that are not mapped. */
  public function fields() {
    return array();
  }
}

/**
 * A migration that is reused for each source CSV file.
 */
class SimpleCSVenMigrate extends SimpleCSVBasicMigration {
  public function __construct($arguments) {
    // Run the parent first
    parent::__construct($arguments);



    // Primary Key
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'UUID' => array('type' => 'varchar', // This field is used to track nodes between imports
                         'length' => 6,
                         'not null' => TRUE,
                         'description' => 'UUID',
                        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Field Mapping for English
    $this->addFieldMapping('body', 'description-en');
    $this->addFieldMapping('language')
         ->defaultValue('en');
    $this->addFieldMapping(NULL, 'description-fr')
         ->description('Not needed yet.');
  }
}



/**
 * A migration that is reused for each source CSV file.
 */
class SimpleCSVfrMigrate extends SimpleCSVBasicMigration {
  public function __construct($arguments) {
    // Run the parent first
    parent::__construct($arguments);



    // Primary Key
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'UUID' => array('type' => 'varchar', // This field is used to track nodes between imports
                         'length' => 6,
                         'not null' => TRUE,
                         'description' => 'UUID',
                        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Declare English migration as a dependency
    $this->dependencies = array('SimpleCSVenMigrate');

    // Field Mapping for French
    $this->addFieldMapping('tnid', 'UUID')
         ->sourceMigration('SimpleCSVenMigrate')
         ->description('Connecting this node to the english default version.');
    $this->addFieldMapping(NULL, 'description-en');
    $this->addFieldMapping('language')
         ->defaultValue('fr');
    $this->addFieldMapping('body', 'description-fr')
         ->description('Not needed yet.');
  }
}
